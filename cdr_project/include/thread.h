#include <stdlib.h>
#include <pthread.h>

typedef void *(*Work_Func) (void *);

typedef enum  Thread_Result
{
    THREAD_SUCCESS,
    THREAD_FAIL
}Thread_Result;

Thread_Result Create_Threads(pthread_t *threads, size_t numOfThreads, Work_Func _func, void *_data);

void Join_Threads(int _numOfThreads,  pthread_t* _pTreads);
        
void Cancel_Threads(int _numOfThreads,  pthread_t* _pTreads);
 

