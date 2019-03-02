#include "bringup.h"
#include "device_factory.h"

#include "fire_sensor.h"
#include "sprinkle_controller.h"

#include <iostream>

using namespace std;
using namespace SH;

int main()
{
    
    // Start Up:
    Bringup bringup;

    // Create Sensors:
    DeviceFactory factory;
    Sensor* fire0 = factory.MakeSensor("lib/dlFireSensor.so",2,3);
    Sensor* fire1 = factory.MakeSensor("lib/dlFireSensor.so",1,3);

    // Connect Sensors:
    bringup.LoadDevice(fire0);
    bringup.LoadDevice(fire1);

    // Connect Controllers:
    Subscriber& subscriber = bringup.GetSubscriber();
	SprinkleController sprinkle0("Sprinkle0");
    sprinkle0.RegisterForEvents(subscriber, 2, 3);
   
	SprinkleController sprinkle1("Sprinkle1");
    sprinkle1.RegisterForEvents(subscriber, 2);

	SprinkleController sprinkle2("Sprinkle2");
    sprinkle2.RegisterForEvents(subscriber, 1, 3);
  
    // Smart House is ON
    cout << endl << "Smart House is Running, Press Any Key to Stop" << endl << endl;
    bringup.Run();

    int stopKey;
    std::cin >> stopKey;

    bringup.Stop();

    return 0;
}
