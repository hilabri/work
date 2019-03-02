#include "receiver.h"

using namespace SH;

// TODO handle exceptions 

Receiver::Receiver() {}


void Receiver::PushEvent(const Event& _event)
{
    m_eventsQ.Push(_event);
}

void Receiver::PopEvent(Event& _event)
{
    m_eventsQ.Pop(_event);
}



