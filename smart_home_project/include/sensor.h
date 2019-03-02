#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "receiver.h"
#include "waitable_queue/runnable.h"

namespace SH{

class Sensor : public Runnable
{
public:
	Sensor(const ID& _id);
	virtual ~Sensor() {}

    void Run(); 

    virtual void GeneratePayload(Event& _event) const =0;

    void ConnectSensor(Receiver* _receiver);
private:
    void PublishEvent();

private:
    const ID m_id; 
    Receiver* m_receiver;
    // string log TODO could be a separate device that subscribe for all events
    // string config 
};





}

#endif
