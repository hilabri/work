#ifndef __MUTEX__
#define __MUTEX__

#include <stdexcept>
#include <pthread.h>
#include <assert.h>


namespace SH{

class Mutex
{
public:	
    enum MutexType 
    {
        Normal = PTHREAD_MUTEX_NORMAL,
        Errocheck = PTHREAD_MUTEX_ERRORCHECK,
        Recursive = PTHREAD_MUTEX_RECURSIVE
    };
	
    Mutex(MutexType _type = Recursive);
    ~Mutex();
    void Lock();
    void UnLock();
  
private:
    Mutex(const Mutex& _mutex);
    Mutex& operator = (const Mutex& _mutex);

private:
    mutable pthread_mutex_t m_mutex;
};


}


#endif
