#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdexcept>
#include <pthread.h>
#include <assert.h>
#include "runnable.h"
#include <iostream>

namespace SH{

class Thread
{
public:
    enum ThreadMode 
    {
        Joinable = PTHREAD_CREATE_JOINABLE,
	    Detached = PTHREAD_CREATE_DETACHED
    };	

    Thread(Runnable& _runnable, ThreadMode _mode = Joinable);
    ~Thread();
    void Join();
    void Cancel();
  
private:
    static void* Thread_func(void* _pv);
  
private:
    Runnable& m_runnable;
    pthread_t m_thread;
    bool m_shouldJoin;
};

}
#endif/*__THREAD_H__*/
