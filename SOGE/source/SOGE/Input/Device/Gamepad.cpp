#include "sogepch.hpp"

#include "SOGE/Input/Device/Gamepad.hpp"


namespace soge
{
    Gamepad::Gamepad(const eastl::string_view aGamepadName) : InputDevice(aGamepadName, DeviceType::DeviceType_Gamepad)
    {
    }
}
