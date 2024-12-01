#include "sogepch.hpp"
#include "SOGE/Input/Device/Mouse.hpp"


namespace soge
{
    Mouse::Mouse(eastl::string_view aMouseName) : InputDevice(aMouseName, DeviceType::DeviceType_Mouse)
    {
    }
}
