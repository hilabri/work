#include "subscriber.h"

using namespace SH;

  
void Subscriber::SubscribeController(ID& _id, Controller* _controller)
{
    m_registration.Insert(_id, _controller);
}

void Subscriber::UnsubscribeController(ID& _id, Controller* _controller)
{
    m_registration.Remove(_id, _controller);
}
    
void Subscriber::RetrieveSubscribers(const ID& _id, std::set<Controller*>& _outputSet) const
{
    typedef std::set<Controller*> Set;
    Set perRoom = m_registration.Retrieve(_id);
    Set::iterator it = perRoom.begin();
    while (it != perRoom.end())
    {
        _outputSet.insert(*it);
        ++it;
    }
    Set perFloor = m_registration.Retrieve(AllFloor(_id));
    it = perFloor.begin();
    while (it != perFloor.end())
    {
        _outputSet.insert(*it);
        ++it;
    }
}
