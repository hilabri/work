#ifndef __RECEIVER_H__
#define __RECEIVER_H__

#include "event.h"
#include "waitable_queue/waitable_queue.h"

namespace SH{

class Receiver
{
public:
	Receiver();
	//~Receiver();
  
	void PushEvent(const Event& _event);
    void PopEvent(Event& _event);
 
private:
	 WaitableQueue<Event> m_eventsQ;
};




}
#endif
