#include "sogepch.hpp"
#include "SOGE/Input/Device/Keyboard.hpp"


namespace soge
{
    Keyboard::Keyboard(eastl::string_view aKeyboardName) : InputDevice(aKeyboardName, DeviceType::DeviceType_Keyboard)
    {
    }
}
