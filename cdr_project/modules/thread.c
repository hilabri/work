#include "thread.h"


Thread_Result Create_Threads(pthread_t *threads, size_t numOfThreads, void *(*Work_Func)(void*), void *_data)
{
    int i;
    for (i = 0; i < numOfThreads; i++)
    {
        if(0 != (pthread_create(&threads[i], NULL, Work_Func, _data))) 
        {
            return THREAD_FAIL;    
        }
    }
    return THREAD_SUCCESS;
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


