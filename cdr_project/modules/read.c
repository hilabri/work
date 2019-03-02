#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "waitable_queue.h"
#include "read.h"
#include "zlog4.h"
#include <libgen.h> /* dirname */
#include <sys/stat.h> /* mkdir */
#define _GNU_SOURCE  

#define TRUE 1
#define FALSE 0

#define MODULE "read"

static void* Read_StoreLines(void *_read);

struct Read
{
    Queue *m_linesQ;
    Detect *m_detect;
    char *m_doneDir;
    int m_isRunning;
    size_t m_threadsNum;
    pthread_t *m_threads;
    pthread_cond_t m_cond;
   	pthread_mutex_t m_mutex;

};

/*--------------------------------------------------*/

Read *Read_Create(size_t _linesCapacity, size_t _readThreadNum, char* _doneDirectory, Detect *_detect)
{
    int res;
    Read *read = malloc(sizeof(Read));
    if(!read)
    {
        ZLOG(LOG_ERROR, MODULE, "Read failed to allocate memory");
        return NULL;
    }
    read->m_detect = _detect;
    read->m_doneDir = _doneDirectory;
    read->m_linesQ = Queue_Create(_linesCapacity);
    if(!read->m_linesQ)
    {
        free(read);
        ZLOG(LOG_ERROR, MODULE, "Read failed to create lines queue");
        return NULL;
    }
    read->m_threadsNum = _readThreadNum;
    read->m_threads = malloc(_readThreadNum * sizeof(pthread_t));
    if(!read->m_threads)
    {
        free(read->m_linesQ);
        free(read);
        ZLOG(LOG_ERROR, MODULE, "Read failed to allocate memory for threads");
        return NULL;
    }
    res = mkdir(_doneDirectory, 0777);
    if (0==res)
    {
        ZLOG(LOG_TRACE, MODULE, "Done dir %s was created\n", _doneDirectory);
    }
    pthread_mutex_init(&read->m_mutex, NULL);
	pthread_cond_init(&read->m_cond,NULL);
    read->m_isRunning = FALSE; 
    ZLOG(LOG_INFO, MODULE, "Read created");      
    return read; 
}
/*--------------------------------------------------*/

void Read_Stop(Read *_read)
{
    if(!_read)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    Cancel_Threads(_read->m_threadsNum, _read->m_threads);
	Join_Threads(_read->m_threadsNum, _read->m_threads);
	ZLOG(LOG_INFO, MODULE, "Read is now stoped");
}

/*--------------------------------------------------*/
static void ElementDestroy(void *_line)
{
    free((char*)_line);
}

/*--------------------------------------------------*/

void Read_Destroy(Read **_read)
{
    if(!_read || !(*_read))
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    free((*_read)->m_threads);
    Queue_Destroy(&((*_read)->m_linesQ), ElementDestroy);
    pthread_cond_destroy(&(*_read)->m_cond);
	pthread_mutex_destroy(&(*_read)->m_mutex);
    free(*_read);
    *_read = NULL;
    ZLOG(LOG_INFO, MODULE, "Read destroy done");
}

/*--------------------------------------------------*/
void Read_Pause(Read *_read)
{
    pthread_mutex_lock(&_read->m_mutex);
    if(_read->m_isRunning)
    {
        _read->m_isRunning = FALSE;
    }
    pthread_mutex_unlock(&_read->m_mutex);
     ZLOG(LOG_INFO, MODULE, "Read is now paused");
}
/*--------------------------------------------------*/
Read_Result Read_Resume(Read *_read)
{
     pthread_mutex_lock(&_read->m_mutex);
    if(!_read->m_isRunning)
    {
        _read->m_isRunning = TRUE;
        pthread_cond_broadcast(&_read->m_cond);
    }
    pthread_mutex_unlock(&_read->m_mutex);
    ZLOG(LOG_INFO, MODULE, "Read is now resumed");
    return READ_SUCCESS;
}

/*--------------------------------------------------*/
static int ShouldRun(Read *_read)
{
    pthread_mutex_lock(&_read->m_mutex);  
    if(!_read->m_isRunning)
    {
        pthread_cond_wait(&_read->m_cond, &_read->m_mutex);
    }
    pthread_mutex_unlock(&_read->m_mutex);
    return 1;    
}
/*--------------------------------------------------*/
static void* Read_StoreLines(void *_read)
{
    FILE *file;
    char *line, *bname, *path;
    Read *read = _read;
    char tempLine[MAX_LINE_SIZE];
    char Pfile[MAX_NAME_SIZE];
    char newName[MAX_NAME_SIZE];
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); 
    while(ShouldRun(read)) 
    {
        ZLOG(LOG_TRACE, MODULE, "Read waiting for files");
        if(DETECT_SUCCESS == Detect_GetFile(read->m_detect, Pfile))
        {
            file = fopen(Pfile, "r");
            if(file)
            {
                while(fgets(tempLine, MAX_LINE_SIZE, file))
                {
		            line = malloc(MAX_LINE_SIZE * sizeof(char));
		            if(!line)
		            {
    		            ZLOG(LOG_ERROR, MODULE, "Read failed to allocate memory for line");
    		            return NULL;
		            }
		            strcpy(line, tempLine);
                    Queue_Enqueue(read->m_linesQ, line);
                }
                fclose(file);
                /* move file to done dir */
                ZLOG(LOG_TRACE, MODULE, "Moving file to done directory");   
				path = Pfile;
				bname = basename(path); /* basename */
				strcpy(newName, read->m_doneDir);
				strcat(newName, bname);
       			rename(Pfile,newName); 
            }
        }
    }
    ZLOG(LOG_TRACE, MODULE, "Read stored lines");
    return NULL; 
}


/*--------------------------------------------------*/

Read_Result Read_Run(Read *_read)
{
    if (!_read)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return READ_UNINITIALIZED; 
    }
    _read->m_isRunning = TRUE;
    if(THREAD_SUCCESS != Create_Threads(_read->m_threads, _read->m_threadsNum, Read_StoreLines, _read))
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create threads");
        return READ_THREAD_FAIL;
    }
    ZLOG(LOG_INFO, MODULE, "Read is now running");
    return READ_SUCCESS;
}

/*--------------------------------------------------*/

Read_Result Read_GetLine(Read *_read, char *_Pline)
{
    char *line;
    if(!_read)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return READ_UNINITIALIZED;
    }
    if(QUEUE_SUCCESS != Queue_Dequeue(_read->m_linesQ, (void**)&line))
    {
        ZLOG(LOG_SEVERE, MODULE, "returned unsuccessful from dequeue - should wait on queue");
        return READ_GET_FAIL;
    }
    strcpy(_Pline, line); 
    free(line); 
    ZLOG(LOG_TRACE, MODULE, "Read dequeue line");
    return READ_SUCCESS;
}

/*--------------------------------------------------*/





