#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include "waitable_queue.h"
#include "thread.h"
#include "detect.h"
#include <sys/inotify.h>
#include <sys/stat.h> /* mkdir */
#include <unistd.h>
#include "zlog4.h"
#include <errno.h>

#define TRUE 1
#define FALSE 0 

#define MODULE "detect"

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define INOTIFY_BUF_LEN     (1024 * ( EVENT_SIZE + 16 ))

static void Detect_StoreFiles(Detect *_detect);
static void *Detect_CdrFiles(void *_detect);

struct Detect
{
    Queue *m_filesQ;
    int m_isRunning;
    pthread_t *m_threads;
    pthread_mutex_t m_workMutex;
    size_t m_threadsNum;
    char m_mask[MASK_SIZE];
    char m_inputDir[DIR_SIZE];
    char m_progressDir[DIR_SIZE];
    pthread_cond_t m_cond;
   	pthread_mutex_t m_mutex;
};

/*--------------------------------------------*/

Detect *Detect_Create(size_t _filesCapacity, size_t _detectThreadNum, char* _cdrFileMask, char* _inputDirectory, char* _inProgressDirectory)
{
    int res;
    Detect *detect = malloc(sizeof(Detect));
	if(!detect)
	{
	    ZLOG(LOG_ERROR, MODULE, "Detect failed to allocate memory");
		return NULL;
	}	
	detect->m_filesQ = Queue_Create(_filesCapacity);
	if(!detect->m_filesQ)
	{
	    ZLOG(LOG_ERROR, MODULE, "Detect failed to create files queue");
		free(detect);
		return NULL;
	}
    detect->m_threadsNum = _detectThreadNum;
    detect->m_threads = malloc(_detectThreadNum * sizeof(pthread_t));
	if(!detect->m_threads)
	{
	    ZLOG(LOG_ERROR, MODULE, "Detect failed to allocate memory for threads");
		free(detect->m_filesQ);
		free(detect);
		return NULL;
	}
    res = mkdir(_inputDirectory, 0777);
    if (0==res)
    {
        ZLOG(LOG_TRACE, MODULE, "Input dir %s was created", _inputDirectory);
    }
    res = mkdir(_inProgressDirectory, 0777);
    if (0==res)
    {
        ZLOG(LOG_TRACE, MODULE, "In-Progress dir %s was created", _inProgressDirectory);
    }
	pthread_mutex_init(&detect->m_workMutex, NULL);
	pthread_mutex_init(&detect->m_mutex, NULL);
	pthread_cond_init(&detect->m_cond,NULL);
    strcpy(detect->m_mask, _cdrFileMask);
    strcpy(detect->m_inputDir, _inputDirectory);
    strcpy(detect->m_progressDir, _inProgressDirectory);
    detect->m_isRunning = FALSE;
    ZLOG(LOG_INFO, MODULE, "Detect created");
	return detect;
}

/*--------------------------------------------*/

static void Detect_StoreFiles(Detect *_detect)
{
    DIR *mydir;
    struct dirent *myfile;
	int length, renameRes;
	char *fileName;
    char oldName[MAX_NAME_SIZE];
    
    mydir = opendir(_detect->m_inputDir);
    if(!mydir)
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to open directory, errno is %s", strerror(errno));
        return;
    }
    while(1)
    {
        pthread_mutex_lock(&_detect->m_workMutex);
        myfile = readdir(mydir);
        if (!myfile)
        {
            pthread_mutex_unlock(&_detect->m_workMutex);
            ZLOG(LOG_TRACE, MODULE, "End of the directory");
            break;
        }
    	length = strlen(myfile->d_name);
        if (!strcmp((myfile->d_name + length - strlen(_detect->m_mask)), _detect->m_mask))
        {
            ZLOG(LOG_TRACE, MODULE, "found cdr file");
            fileName = malloc(MAX_NAME_SIZE * sizeof(char));
            if(!fileName)
            {
                pthread_mutex_unlock(&_detect->m_workMutex);
                ZLOG(LOG_ERROR, MODULE, "Failed to allocate memory for fileName");
                break;
            }
            strcpy(oldName, _detect->m_inputDir);
            strcat(oldName, myfile->d_name);
            strcpy(fileName, _detect->m_progressDir);
            strcat(fileName, myfile->d_name);
			/* move file to progress dir */
			ZLOG(LOG_TRACE, MODULE, "Moving file to in progress directory");
            renameRes = rename(oldName,fileName);
            pthread_mutex_unlock(&_detect->m_workMutex);
			if (-1 == renameRes)
            {
                pthread_mutex_unlock(&_detect->m_workMutex);
                ZLOG(LOG_ERROR, MODULE, "Rename %s => %s failed, errno is %s", oldName, fileName, strerror(errno)); 
				break;
			}
            Queue_Enqueue(_detect->m_filesQ, fileName); 
        }
        else
        {
            ZLOG(LOG_TRACE, MODULE, "file %s didn't match mask %s", myfile->d_name, _detect->m_mask);
            pthread_mutex_unlock(&_detect->m_workMutex);
        }
    }
    closedir(mydir);
    ZLOG(LOG_TRACE, MODULE, "Detect stored file");
}

/*--------------------------------------------*/

void Detect_Pause(Detect *_detect)
{
    pthread_mutex_lock(&_detect->m_mutex);
    if(_detect->m_isRunning)
    {
        _detect->m_isRunning = FALSE;
    }
    pthread_mutex_unlock(&_detect->m_mutex);
    ZLOG(LOG_INFO, MODULE, "Detect is now paused");
}
/*--------------------------------------------------*/
Detect_Result Detect_Resume(Detect *_detect)
{
     pthread_mutex_lock(&_detect->m_mutex);
    if(!_detect->m_isRunning)
    {
        _detect->m_isRunning = TRUE;
        pthread_cond_broadcast(&_detect->m_cond);
    }
    pthread_mutex_unlock(&_detect->m_mutex);
    ZLOG(LOG_INFO, MODULE, "Detect is now resumed");
    return DETECT_SUCCESS;
}

/*--------------------------------------------------*/
static int ShouldRun(Detect *_detect)
{
    pthread_mutex_lock(&_detect->m_mutex);  
    if(!_detect->m_isRunning)
    {
        pthread_cond_wait(&_detect->m_cond, &_detect->m_mutex);
    }
    pthread_mutex_unlock(&_detect->m_mutex);
    return 1;    
}
/*--------------------------------------------------*/
static void *Detect_CdrFiles(void *_detect)
{
    int inotifyFd;
    char buf[INOTIFY_BUF_LEN];
    Detect *detect = _detect;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); 
    inotifyFd = inotify_init();
    if(-1 == inotifyFd)
    {
        ZLOG(LOG_ERROR, MODULE, "inotify failed");
        return NULL;
    }
    
    /* watch the input directory for file creations */
    ZLOG(LOG_TRACE, MODULE, "watch the input directory for file creations");
    inotify_add_watch(inotifyFd, detect->m_inputDir, IN_CREATE | IN_MOVED_TO);    

    while(ShouldRun(detect))
    {
        ZLOG(LOG_TRACE, MODULE, "Detect storing file");
        Detect_StoreFiles(detect);
        read(inotifyFd, buf, INOTIFY_BUF_LEN);
    }
    return NULL;
}

/*------------------------------------------------------*/

Detect_Result Detect_Run(Detect *_detect)
{
    if (!_detect)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return DETECT_UNINITIALIZED;
    }
    _detect->m_isRunning = TRUE;
    if(THREAD_SUCCESS != Create_Threads(_detect->m_threads, _detect->m_threadsNum, Detect_CdrFiles, _detect))
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create threads");
        return DETECT_THREAD_FAIL;
    }
    ZLOG(LOG_INFO, MODULE, "Detect is now running");
    return DETECT_SUCCESS;    
}

/*------------------------------------------------------*/
void Detect_Stop(Detect *_detect)
{
	if(!_detect)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    Cancel_Threads(_detect->m_threadsNum, _detect->m_threads);
    Join_Threads(_detect->m_threadsNum, _detect->m_threads);
    ZLOG(LOG_INFO, MODULE, "Detect is now stoped");
}
/*------------------------------------------------------*/

static void ElementDestroy(void *_fileName)
{
    free((char*)_fileName);
}

/*-------------------------------------------------------*/

void Detect_Destroy(Detect* *_detect)
{
	if(!_detect || !(*_detect))
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    pthread_mutex_destroy(&(*_detect)->m_workMutex); 
    free((*_detect)->m_threads);
    Queue_Destroy(&((*_detect)->m_filesQ), ElementDestroy);
    pthread_cond_destroy(&(*_detect)->m_cond);
	pthread_mutex_destroy(&(*_detect)->m_mutex);
	free(*_detect);
	*_detect = NULL;
	ZLOG(LOG_INFO, MODULE, "Detect destroy done");
}

/*------------------------------------------------------*/

Detect_Result Detect_GetFile(Detect *_detect, char *_Pfile)
{
    char *fileName;
    if(!_detect)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return DETECT_UNINITIALIZED;
    }
    if(QUEUE_SUCCESS != Queue_Dequeue(_detect->m_filesQ, (void**)&fileName))
    {
        ZLOG(LOG_SEVERE, MODULE, "returned unsuccessful from dequeue - should wait on queue");
        return DETECT_GET_FAIL;
    }
    strcpy(_Pfile, fileName);
    free(fileName);
    ZLOG(LOG_TRACE, MODULE, "Detect got file");
    return DETECT_SUCCESS;
}
