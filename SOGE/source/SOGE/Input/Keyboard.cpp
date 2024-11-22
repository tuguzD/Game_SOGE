#include "sogepch.hpp"
#include "SOGE/Input/Keyboard.hpp"


namespace soge
{
    Keyboard::Keyboard(eastl::string_view aKeyboardName) : InputDevice(aKeyboardName, DeviceType::DeviceType_Keyboard)
    {
    }
}
