#ifndef __DISPATCHER_H__
#define __DISPATCHER_H__

#include "subscriber.h"
#include "receiver.h"
#include "waitable_queue/runnable.h"

namespace SH{

class Dispatcher : public Runnable
{
public:
	Dispatcher(Subscriber& _subscriber, Receiver& _receiver);
    ~Dispatcher() {}
  
    virtual void Run(); 
    void Stop();

private:
	void DispatchEvent();  
private:
    Subscriber& m_subscriber; 
    Receiver& m_receiver;
    bool m_shouldRun;
};




}
#endif
