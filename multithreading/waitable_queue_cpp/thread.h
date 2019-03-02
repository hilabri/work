#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdexcept>
#include <pthread.h>
#include <assert.h>
#include "runable.h"
#include <iostream>

namespace advcpp{
namespace SH{

class Thread
{
public:
    enum ThreadMode 
    {
        Joinable = PTHREAD_CREATE_JOINABLE,
	Detached = PTHREAD_CREATE_DETACHED
    };	

    Thread(Runable& _runable, ThreadMode _mode = Joinable);
    ~Thread();
    void Join();
    void Cancel();
  
private:
    static void* Thread_func(void* _pv);
  
private:
    Runable& m_runable;
    pthread_t m_thread;
    bool m_shouldJoin;
};


Thread::Thread(Runable& _runable, ThreadMode _mode)
	: m_runable(_runable)
	, m_shouldJoin(_mode == Joinable)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);	
    pthread_attr_setdetachstate(&attr, _mode);

    const int res = pthread_create( &m_thread, &attr, Thread_func, this);

    pthread_attr_destroy(&attr);

    if(0 != res)
    {
	throw std::runtime_error("pthread create failed");
    }
}


Thread::~Thread()
{
    if(m_shouldJoin)
    {
	try
        {
	    Join();
        }
        catch (std::exception& ex)
        {
            // Don't propogate exception from the destructor
	}
    }
}


void Thread::Join()
{
    if(!m_shouldJoin)
    {
	throw std::runtime_error("cannot join thread");
    }
    
    const int res = pthread_join(m_thread, NULL);
    m_shouldJoin = false;

    if(0 != res)
    {
	throw std::runtime_error("pthread join failed");
    }
}


void Thread::Cancel()
{
	const int res = pthread_cancel(m_thread);
	if(0 != res)
	{
		throw std::runtime_error("pthread cancel failed");
	}
}



void* Thread::Thread_func(void* _pv)
{
	Thread* pt = static_cast<Thread*>(_pv);
	try {
		pt->m_runable.Run();
	}
	catch (...) 
	{
		assert("thread run falied");
	}
    return _pv;
}


}}
#endif/*__THREAD_H__*/
