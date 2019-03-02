#ifndef __MUTEX__
#define __MUTEX__

#include <stdexcept>
#include <pthread.h>
#include <assert.h>


namespace advcpp{
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


Mutex::Mutex(MutexType _type)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, _type);
  
    const int res = pthread_mutex_init(&m_mutex, &attr);
  
    pthread_mutexattr_destroy(&attr);

    if(0 != res)
    {
        throw std::runtime_error("initiate mutex failed");
    }
}


Mutex::~Mutex()
{
    const int res = pthread_mutex_destroy(&m_mutex);
    assert(0 == res); 
}


void Mutex::Lock()
{
    const int res =pthread_mutex_lock(&m_mutex);
    if(0 != res)
    {
        throw std::runtime_error("lock mutex failed");
    }
}


void Mutex::UnLock()
{
    const int res = pthread_mutex_unlock(&m_mutex);
    if(0 != res)
    {
        throw std::runtime_error("unlock mutex failed");
    } 
}


}
}

#endif
