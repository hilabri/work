#ifndef __DEVICE_FACTORY_H__
#define __DEVICE_FACTORY_H__

#include "sensor.h"

namespace SH{

class DeviceFactory {
public:
    DeviceFactory() {}
    //~DeviceFactory();

    Sensor *MakeSensor(std::string library, int _floor, unsigned int _room);
private:
};

}

#endif
