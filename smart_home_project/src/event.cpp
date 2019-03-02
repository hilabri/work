#include "event.h"

using namespace SH;

Event::Event(const ID& _id)
    : m_id (_id)
    , m_timeStamp( std::time(0))
{}


void Event::Payload(const std::string _payload)
{
    m_payload = _payload;
}


const ID& Event::Id() const
{
    return m_id;
}


const time_t& Event::TimeStamp() const
{
    return m_timeStamp;
}


const std::string& Event::Payload() const
{
    return m_payload;
}



