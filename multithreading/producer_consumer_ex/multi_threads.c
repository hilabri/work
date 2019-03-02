#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>
#include <getopt.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include "queue.h"

#define DEFAULT_PRODUCERS 5
#define DEFAULT_CONSUMERS 3
#define DEFAULT_SIZE_OF_QUEUE 30
#define DEFAULT_PRODUCE_NUMBER 10
#define M_SEC 1000


typedef struct
{
    int m_Producers;
    int m_Consumers;
    int m_QueueSize;
    int m_work;
    int m_numOfItems; 
} Flags;

typedef struct
{
	Queue *queue;
	Flags flags;
	sem_t semFull;
	sem_t semEmpty;
	pthread_mutex_t mutex;
} WorkInfo;

 /*--------------------------------------------------------------*/
void Getopt(int argc, char **argv, Flags *flags)
{
    int opt;
    
    flags->m_Producers = DEFAULT_PRODUCERS;
    flags->m_Consumers = DEFAULT_CONSUMERS;
    flags->m_QueueSize = DEFAULT_SIZE_OF_QUEUE;
    flags->m_work = 1000*M_SEC;
    flags->m_numOfItems = DEFAULT_PRODUCE_NUMBER; 
       
    while ((opt = getopt(argc, argv, "p:c:q:s:n")) != -1)
    {
        switch (opt)
        {
            case 'p':
                flags->m_Producers = atoi(optarg) * M_SEC; 
                break;
            case 'c':
                flags->m_Consumers = atoi(optarg);
                break;
            case 'q':
                flags->m_QueueSize = atoi(optarg);
                break;
			case 's':
                flags->m_work = atoi(optarg);
                break;
            case 'n':
                flags->m_numOfItems = atoi(optarg);
                break;           
                printf("Usage: - \n");
                break;
        }
    }
 }
 /*--------------------------------------------------------------*/
 
void* Producer_Work(void *_data)
{
    int ItemsToProduce, sleepTime, i;
    pthread_t threadID = pthread_self();       
    WorkInfo *data = (WorkInfo *)_data;
    if (!data)
    {
        return 0;
    }
    ItemsToProduce = rand() % (data->flags.m_numOfItems);
	srand(time(0));
	sleepTime = rand() % (data->flags.m_work);
	for (i = 0; i < ItemsToProduce; i++)
	{		
		usleep(sleepTime);
		if (sem_wait(&(data->semEmpty)) == -1)
		{
       		perror("sem_wait_empty Producer");
			return NULL;
		}
		pthread_mutex_lock(&data->mutex);
		Queue_Enqueue(data->queue, "Message" /* TODO */);   
		printf("Producer %ld added item %d/%d\n", (long)threadID, i+1, ItemsToProduce);
        pthread_mutex_unlock(&data->mutex);
		if (sem_post(&data->semFull) == -1)
		{
			perror("sem_init_full Producer");
			return NULL;
		}
	}
	return (void *)data;
}

 /*--------------------------------------------------------------*/
 
void* Consumer_Work(void *_data)
{
	int sleepTime;
    char *message;
	pthread_t threadID;
	WorkInfo *data = (WorkInfo *)_data;
	if (!data)
    {
        pthread_exit(NULL);
    }
    threadID = pthread_self();
    srand(time(0));
	sleepTime = rand() % (data->flags.m_work); 
	while(1)
    {
        if (sem_wait(&(data->semFull)) == -1)
        {
            perror("sem_wait_full Consumer");                           
            pthread_exit(&errno);
        }
        pthread_mutex_lock(&data->mutex);
       	Queue_Dequeue(data->queue, (void **)&message);
        printf("Consumer %ld received: %s\n", (long)threadID, message);
        pthread_mutex_unlock(&data->mutex);
        if (sem_post(&data->semEmpty) == -1)
        {
            perror("sem_post_empty Consumer");
            pthread_exit(&errno);
        }
        usleep(sleepTime);
    }
    return (void *)data;
}

/*--------------------------------------------------------------*/
pthread_t *Create_Threads(void *(*Work_Func)(void*), WorkInfo *_data, int numOfThreads)
{
    int i;

	pthread_t *threads = malloc(numOfThreads*(sizeof(pthread_t)));
	if (threads)
	{
		for (i=0; i<numOfThreads; i++)
		{
			if((pthread_create(&threads[i], NULL, Work_Func, _data)) != 0) 
			{
				perror("Create_Threads error");
				return NULL;	
			}
		}
	}
	return threads;
}

/*--------------------------------------------------------------*/
void Join_Threads(int _numOfThreads,  pthread_t* _pTreads)
{
	int i;
	void* retval;
	
	for (i=0; i<_numOfThreads; i++)
	{
		pthread_join(_pTreads[i], &retval);
	}
}

/*--------------------------------------------------------------*/

void Cancel_Threads(int _numOfThreads,  pthread_t* _pTreads)
{
	int i;
	
	for (i=0; i<_numOfThreads; i++)
	{
		pthread_cancel(_pTreads[i]);
	}
}

/*--------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	WorkInfo data;
	Queue *queue;
    Flags *flags = &data.flags;
	pthread_t *prodcersThreads; 
	pthread_t *consumersThreads; 
	
    Getopt(argc, argv, flags);
	
	queue = Queue_Create(flags->m_QueueSize);
	if(!queue)
	{
		perror("Queue_Create");
		return errno;
	}
	
	if((sem_init(&data.semEmpty, 0, flags->m_QueueSize)) == -1)
	{
		perror("sem_init semFull");
		return errno;
	}
	if((sem_init(&data.semFull, 0, 0)) == -1)
	{
		perror("sem_init semEmpty");
		return errno;
	}
	pthread_mutex_init(&data.mutex, NULL);

	data.queue = queue;

	prodcersThreads = Create_Threads(Producer_Work, &data, flags->m_Producers);
	if (!prodcersThreads)
	{
		printf("Error - ProdcersThreads Create");
		return 1;
	}
	consumersThreads = Create_Threads(Consumer_Work, &data, flags->m_Consumers); 
	if (!consumersThreads)
	{
		free(consumersThreads);
		printf("Error - ConsumersThreads Create");
		return 1;
	}
	
	Join_Threads(flags->m_Producers, prodcersThreads);
	Cancel_Threads(flags->m_Consumers, consumersThreads);
	Join_Threads(flags->m_Consumers, consumersThreads);
	
	if(Queue_IsEmpty(queue))
	{
		printf("your queue is empty\n");
	}
	else
	{
		printf("your queue is NOT empty\n");
	}
	
    sem_destroy(&data.semFull);
    sem_destroy(&data.semEmpty);
    pthread_mutex_destroy(&data.mutex);
	
	free(prodcersThreads);
	free(consumersThreads);		
   return 0;
}
