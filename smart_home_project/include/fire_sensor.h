#ifndef __FIRE_SENSOR_H__
#define __FIRE_SENSOR_H__

#include "sensor.h"

 
namespace SH{

class FireSensor : public Sensor
{
public:
	FireSensor(int _floor, unsigned int room);
    virtual ~FireSensor() {}
    
    virtual void GeneratePayload(Event& _event) const;

private:
};





}

#endif

