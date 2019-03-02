#include "device_factory.h"
#include <dlfcn.h>

using namespace SH;

Sensor* DeviceFactory::MakeSensor(std::string _library, int _floor, unsigned int _room)
{
    // Load dynamic library
	void *handle = dlopen(_library.c_str(), RTLD_LAZY);
	if (!handle) 
	{
		throw std::runtime_error("dl open failed");
	}
     
    // Return the address of symbol "Create" (function)
    void *funcPtr = dlsym(handle, "Create");
    if (!funcPtr)  
	{
		throw std::runtime_error("dl load symbol failed");
    }

    // Function pointer initialized with the loaded symbol
    typedef Sensor *(*CreateFunc)(int, unsigned int);

    CreateFunc CreateSensor = (CreateFunc)funcPtr;
    
	// Create sensor using the creation function
    Sensor *sensor = CreateSensor(_floor,_room);

    return sensor;
}
