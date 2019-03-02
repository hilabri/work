#include "sensor.h"
#include <unistd.h>

using namespace SH;

Sensor::Sensor(const ID& _id)
    : m_id(_id)
{} 

void Sensor::ConnectSensor(Receiver* _receiver)
{
     m_receiver = _receiver;
}

void Sensor::PublishEvent()
{
    Event newEvent(m_id);
    GeneratePayload(newEvent);
    if(!m_receiver)
    {
        throw std::runtime_error("sensor is not connected to smart house");
    }
    m_receiver->PushEvent(newEvent);
}

void Sensor::Run()
{
    while(true)
    {
        sleep(2);
        PublishEvent();
    }
}
