#ifndef __WAITABLE_QUEUE_H__
#define __WAITABLE_QUEUE_H__

#include <queue>
#include "semaphore.h"
#include "locker.h"
#include "mutex.h" 

namespace SH{

template <class T>
class WaitableQueue
{
public:
    WaitableQueue();
    //~WaitableQueue();

    bool Empty() const;
    void Push(const T& _val);
    void Pop(T& _val);
    
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
	: m_semaphore("queueSem", 0)
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
void WaitableQueue<T>::Pop(T& _val)
{
    T value;
    m_semaphore.Wait();
    try 
    {
        Locker locker(m_mutex);
        _val = m_queue.front();
	    m_queue.pop();
    }
    catch (...)
    {
        m_semaphore.Post();
        throw;
    }
}



}

#endif
