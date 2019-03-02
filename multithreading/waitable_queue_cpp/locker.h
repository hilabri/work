#ifndef __LOCKER_H_
#define __LOCKER_H_

#include "mutex.h"

namespace advcpp{
namespace SH{

class Locker
{
public:
    Locker(Mutex& _mutex);
    ~Locker();
private:
    Mutex& m_mutex;
};


inline Locker::Locker(Mutex& _mutex)
    :m_mutex(_mutex)
{
    // exceptions will propogate to caller
    m_mutex.Lock();
}


inline Locker::~Locker()
{
    try 
    {
        m_mutex.UnLock();
    } 
    catch (std::exception& ex) 
    {
        assert("mutex unlock falied");
    }
}


}}
#endif
