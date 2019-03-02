#ifndef __SUBSCRIBER_H__
#define __SUBSCRIBER_H__

#include "registration.h"
#include "id.h"

namespace SH{

// Forward declaration:
class Controller;

class Subscriber
{
public:
	Subscriber() {}
	//~Subscriber();
  
	void SubscribeController(ID& _id, Controller* _controller);
	void UnsubscribeController(ID& _id, Controller* _controller);
    
    typedef std::set<Controller*> Set;
    void RetrieveSubscribers(const ID& _id, Set& _outputSet) const;
  
private:
	Registration<ID, Controller*> m_registration;
};



}
#endif
