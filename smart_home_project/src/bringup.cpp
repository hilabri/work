#include "bringup.h"

using namespace SH;

Bringup::Bringup()
    : m_dispatcher(m_subscriber, m_receiver)
{
}

void Bringup::Run()
{
    Thread *tDispatcher = new Thread(m_dispatcher);
    m_threads.push_back(tDispatcher);
}

void Bringup::LoadDevice(Sensor *_sensor)
{
    _sensor->ConnectSensor(&m_receiver);
    Thread *tSensor = new Thread(*_sensor);
    m_threads.push_back(tSensor);
}

void Bringup::Stop()
{
    const size_t count = m_threads.size();
    for (unsigned int i = 0; i < count; i++)
    {
        m_threads[i]->Cancel();
    }
    while (!m_threads.empty())
    {
        Thread *thread = m_threads.back();
        m_threads.pop_back();
        thread->Join();
        delete thread;
    }
}

Subscriber& Bringup::GetSubscriber()
{
    return m_subscriber;
}
