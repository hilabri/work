
#include "controller.h"
 
using namespace SH;

Controller::Controller(const std::string _type)
    : m_type(_type)
{}


void Controller::RegisterForEvents(Subscriber& _subscriber, int _floor)
{
    ID id(m_type, _floor);    
    _subscriber.SubscribeController(id, this);
}


void Controller::RegisterForEvents(Subscriber& _subscriber, int _floor, unsigned int _room)
{
    ID id(m_type, _floor, _room);    
    _subscriber.SubscribeController(id, this);
}



