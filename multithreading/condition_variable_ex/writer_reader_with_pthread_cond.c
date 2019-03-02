#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define  DEFAULT_NUM_LOOPS 3
#define DEFAULT_NUM_READERS 2
#define ARR_SIZE 10

#define TRUE  1
#define FALSE 0

typedef struct
{
    int m_whileFlag; 
    int m_numLoops;
    int m_numReaders;
    int m_verbose;
} Flags;	



typedef struct
{
	pthread_cond_t m_cond;
	Flags* m_flags; 
	int m_counterMessages;
	int m_counterReaders;
   	pthread_mutex_t m_mutex;
   	int buffer;
}WorkInfo;


typedef void* (*ThreadFunc)(void* _info); 
int numArray[ARR_SIZE]={1,2,3,4,5,6,7,8,9,10};

/*----------------------------------------------------*/
Flags Getopt(int argc, char *argv[])
{
  	Flags flag;
  	int opt,res;
	_flags->m_verbose = 0;
	_flags->m_numLoops= DEFAULT_NUM_LOOPS;
	_flags->m_numReaders = DEFAULT_NUM_READERS;

 	while ((opt = getopt (argc, argv, "vl:r:")) != -1)
  	{
		switch (opt)
    	{
			case 'v':
				_flags->m_verbose = 1;
				break;
			case 'l':
				if((res= atoi(optarg)) > 0)
				{
					_flags->m_numLoops = res;
				}
				break;	
			case 'r':
				if((res = atoi(optarg)) > 0)
				{
					_flags->m_numReaders = res;
				}
				break;					
			default: 
				fprintf("argvs: -v -l -r\n");
		}
	} 
	return flag;
}

/*----------------------------------------------------*/			
static void* Writer_Func(void* _info)
{
	WorkInfo* info=(WorkInfo*)_info;
	long int threadId=pthread_self();
	if (!_info)
	{
		return NULL;
	}
	printf("Writer thread id = %ld\n",threadId );

	/*lock(mutex)*/
	pthread_mutex_lock(&(info->m_mutex));
	
	while(! info->m_counterReaders)
	{
		printf("writer waiting for a reader\n");
		pthread_cond_wait(&(info->m_cond) , &(info->m_mutex));

	}
	
	printf("writes a message\n");
	/**enter message*/
	info->buffer=numArray[randNum];
	info->m_counterMessages++;
	printf("message is %d\n",info->buffer );
	pthread_cond_broadcast(&(info->m_cond));
	
	/*unlock(mutex)*/
	pthread_mutex_unlock(&(info->m_mutex));
		
	
	return _info;
}

/*----------------------------------------------------*/
static void* Reader_Func(void* _info)
{
	WorkInfo* info=(WorkInfo*)_info;
	long int threadId;
	if (!_info)
	{
		return 0;
	}
	threadId=pthread_self();
	printf("Reader thread id = %ld,\n",threadId);
	
	/*lock(mutex)*/
	pthread_mutex_lock(&(info->m_mutex));
	
	pthread_cond_broadcast(&(info->m_cond));
	
	while(! info-> m_counterMessages )
	{	
		printf("reader waiting for a new message\n");
		pthread_cond_wait(&(info->m_cond) , &(info->m_mutex));
	}
	
	printf("reads a message\n");
	printf("message is %d\n",info->buffer );
	info->m_counterMessages--;
	
	/*unlock(mutex)*/
	pthread_mutex_unlock(&(info->m_mutex));
	
	return _info;
}

/*----------------------------------------------------*/

/*if flag ==1 we are in readers creation*/
pthread_t* ThreadCreate(ThreadFunc _func,int _num, WorkInfo* _info, int _isReader)
{
	pthread_t* threadsArr=NULL;
	int i;
	if (!_info || !_func)
	{
		return NULL;
	}
	
	threadsArr = malloc(_num *sizeof(pthread_t));
	if (!threadsArr)
	{
		return NULL;
	}
	
	/*create threads Id array*/
	for (i=0; i<_num; i++)
	{
		if((pthread_create(&threadsArr[i], NULL , _func, (void*)_info)) != 0)
        {
        	free(threadsArr);
            return NULL;
        }
        if (_isReader)
        {
        	((WorkInfo*)_info)->m_counterReaders++;
        }       
	} 
	return threadsArr;
}	

/*----------------------------------------------------*/		
static void JoinThreads(int _numOfThreads,  pthread_t* _pTreads)
{
	int i;
	void* output;
	
	for (i=0; i<_numOfThreads; i++)
	{
		pthread_join(_pTreads[i],&output);
	}
}

/*----------------------------------------------------*/
static void CancelThreads(int _numOfThreads,  pthread_t* _pTreads)
{
	int i;
	
	for (i=0; i<_numOfThreads; i++)
	{
		pthread_cancel(_pTreads[i]);
	}
}		

/*----------------------------------------------------*/
							
int main(int argc, char *argv[])
{
    int res, i;
    WorkInfo info; 
	pthread_t* readersThreads;
	pthread_t* writerThread;
	Flags flags = Getopt(argc, argv);
	
	info.m_flags=&flags;
	
	printf("num of Readers is %d\n", flags.m_numReaders);
	
	/* initiate mutex */
	pthread_mutex_init(&(info.m_mutex), NULL);
		
	/* init cond */
	pthread_cond_init(&info.m_cond,NULL);
	
	for (i=0;i<flags.m_numLoops;i++)
    {
   		/*counters initiation*/
		info.m_counterMessages=0;
		info.m_counterReaders=0;	
	
		/*create Writer*/
	   	writerThread = ThreadCreate(Writer_Func, flags.m_numReaders, &info, FALSE);  
	   	if (!writerThread)
	   	{
	   		printf("ThreadCreate");
	   		free(readersThreads);
	   		return errno;
	   	}
	   	
	   	/*create Readers*/
		readersThreads = ThreadCreate(Reader_Func, 1, &info, TRUE);
	   	if (!readersThreads)
	   	{
	   		printf("error ThreadCreate");
	   		return errno;
	   	}
	   	
	   	JoinThreads(1, writerThread);	
		CancelThreads(flags.m_numReaders, readersThreads);
		JoinThreads(flags.m_numReaders, readersThreads);

	   	free(readersThreads);
		free (writerThread);	   
    }
       
	pthread_cond_destroy(&(info.m_cond));
	pthread_mutex_destroy(&(info.m_mutex));
	
	return 0;
}



