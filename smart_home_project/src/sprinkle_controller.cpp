#include "sprinkle_controller.h"
#include <iostream>

using namespace SH;

SprinkleController::SprinkleController(const std::string _identification)
    : Controller("FIRE")
    , m_device(_identification)
{}


void SprinkleController::HandleEvent(Event& _event)
{
    std::cout << _event.Payload() << std::endl
              << m_device << " got new event=> "
              << "Type: " << _event.Id().Type()
              << " Location: " << _event.Id().Floor() << " " << _event.Id().Room() << std::endl
              << "Action: start sprinkling water!\n";
}
