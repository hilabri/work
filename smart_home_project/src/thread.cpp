#include "waitable_queue/thread.h"

using namespace SH;

Thread::Thread(Runnable& _runnable, ThreadMode _mode)
	: m_runnable(_runnable)
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
            assert(false);
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
		pt->m_runnable.Run();
	}
	catch (...) 
	{
		assert(false);
	}
    return _pv;
}



