#include "sogepch.hpp"
#include "SOGE/Input/Device/Gamepad.hpp"


namespace soge
{
    Gamepad::Gamepad(eastl::string_view aGamepadName) : InputDevice(aGamepadName, DeviceType::DeviceType_Gamepad)
    {
    }
}
