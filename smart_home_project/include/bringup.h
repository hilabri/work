#ifndef __BRING_UP_H__
#define __BRING_UP_H__

#include "dispatcher.h"
#include "waitable_queue/thread.h"
#include "device_factory.h"
#include <vector>

namespace SH{

class Bringup
{
public:
    Bringup();
    ~Bringup() {}

    void LoadDevice(Sensor *_sensor);
    
    void Run();
    void Stop();

    Subscriber& GetSubscriber();

private:
    std::vector<Thread*> m_threads;
    Subscriber m_subscriber; 
    Receiver m_receiver; 
    Dispatcher m_dispatcher;
};


}
#endif
                                                                                                                                                           
                                                                                                                                                           
