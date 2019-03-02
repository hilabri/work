
#include "dispatcher.h"
#include "controller.h"

#include <iostream>

using namespace SH;

Dispatcher::Dispatcher(Subscriber& _subscriber, Receiver& _receiver)
    : m_subscriber (_subscriber)
    , m_receiver (_receiver)
    , m_shouldRun (false)
{}

void Dispatcher::Run()
{
    m_shouldRun = true;
    DispatchEvent();
}

void Dispatcher::Stop() 
{
    m_shouldRun = false;
}

void Dispatcher::DispatchEvent() 
{
    while(m_shouldRun)
    {
       Event event;
       m_receiver.PopEvent(event);
       std::set<Controller*> set;
       m_subscriber.RetrieveSubscribers(event.Id(), set);
       std::set<Controller*>::iterator it = set.begin();  
       while(it != set.end())
       {
           (*it)->HandleEvent(event);
           it++;
       }
    }
}
  
