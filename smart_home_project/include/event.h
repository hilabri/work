#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>
#include <ctime>
#include "id.h"

namespace SH{

class Event
{
public:
	Event() {}
    Event(const ID& _id);
	//~Event();

    //Event(const Event& _event);
    //Event& operator=(const Event& _event);

	void Payload(const std::string _payload);
        
    const ID& Id() const;
    const time_t& TimeStamp() const;
    const std::string& Payload() const;

private:
	ID m_id;
	time_t m_timeStamp;
    // T? payload 
    std::string m_payload;
};


}
#endif
