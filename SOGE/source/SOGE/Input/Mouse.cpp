#include "sogepch.hpp"
#include "SOGE/Input/Mouse.hpp"


namespace soge
{
    Mouse::Mouse(eastl::string_view aMouseName) : InputDevice(aMouseName, DeviceType::DeviceType_Mouse)
    {
    }
}
