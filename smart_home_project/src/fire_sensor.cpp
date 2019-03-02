#include "fire_sensor.h"

using namespace SH;

// extern "C" makes a function-name in C++ have 'C' linkage (compiler does not mangle the name)
extern "C" Sensor* Create(int _floor, unsigned int _room)
{
    return new FireSensor(_floor, _room);
}

FireSensor::FireSensor(int _floor, unsigned int _room)
    : Sensor(ID("FIRE", _floor, _room))
{}


void FireSensor::GeneratePayload(Event& _event) const
{
    _event.Payload("\033[1;31mFire-Sensor: Fire! Fire!\033[0m\n");
}       

