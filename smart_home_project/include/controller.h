#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "subscriber.h"
#include "event.h"


namespace SH{

class Controller
{
public:
	Controller(const std::string _type);
	virtual ~Controller() {}

    void RegisterForEvents(Subscriber& _subscriber, int _floor);
    void RegisterForEvents(Subscriber& _subscriber, int _floor, unsigned int _room);
	
    virtual void HandleEvent(Event& _event) = 0;
	
private:
    const std::string m_type; 
    // config
    // log TODO could be a separate device that subscribe for all events 
};




}
#endif
