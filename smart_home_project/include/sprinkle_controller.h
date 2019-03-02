#ifndef __SPRINKLE_H__
#define __SPRINKLE_H__

#include "controller.h"


namespace SH{

class SprinkleController : public Controller
{
public:
	SprinkleController(const std::string _identification = "sprinkle_controller");
	virtual ~SprinkleController() {}
	
    virtual void HandleEvent(Event& _event);

private:
    const std::string m_device;
};




}
#endif

