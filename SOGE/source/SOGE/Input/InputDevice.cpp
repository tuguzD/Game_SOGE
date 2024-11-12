#include "sogepch.hpp"
#include "SOGE/Input/InputDevice.hpp"


namespace soge
{
    InputDevice::InputDevice(eastl::string aName, InputDeviceType aType) : m_deviceName(aName), m_deviceType(aType)
    {
    }

    eastl::string InputDevice::GetName()
    {
        return m_deviceName;
    }

    InputDeviceType InputDevice::GetDeviceType()
    {
        return m_deviceType;
    }
}
