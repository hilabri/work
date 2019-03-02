#include "waitable_queue/mutex.h"

using namespace SH;

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


