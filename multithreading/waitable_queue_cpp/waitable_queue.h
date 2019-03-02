#ifndef __WAITABLE_QUEUE_H__
#define __WAITABLE_QUEUE_H__

#include <queue>
#include "semaphore.h"
#include "mutex.h"
#include "locker.h"

namespace advcpp{
namespace SH{

template <class T>
class WaitableQueue
{
public:
    WaitableQueue();
    //~WaitableQueue();

    bool Empty() const;
    void Push(const T& _val);
    T Pop();
    
private:
    WaitableQueue(const WaitableQueue& _wq);
    WaitableQueue& operator=(const WaitableQueue& _wq);
	
private:
    Semaphore m_semaphore;
    Mutex m_mutex;
    std::queue<T> m_queue;
};


template <class T>
WaitableQueue<T>::WaitableQueue()
	: m_semaphore(0)
{
}


template <class T>
bool WaitableQueue<T>::Empty() const
{
    return m_queue.empty();
}


template <class T>
void WaitableQueue<T>::Push(const T& _val)
{
    {
        Locker locker(m_mutex);
        m_queue.push(_val);
    }
    m_semaphore.Post();
}


template <class T>
T WaitableQueue<T>::Pop()
{
    T value;
    m_semaphore.Wait();
    try 
    {
        Locker locker(m_mutex);
        value = m_queue.front();
	m_queue.pop();
    }
    catch (...)
    {
        m_semaphore.Post();
        throw;
    }
    return value;
}



}
}

#endif
