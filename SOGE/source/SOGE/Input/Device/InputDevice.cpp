#include "sogepch.hpp"
#include "SOGE/Input/Device/InputDevice.hpp"


namespace soge
{
    InputDevice::InputDevice(eastl::string_view aDeviceName, DeviceType aDeviceType)
        : m_deviceName(aDeviceName), m_deviceType(aDeviceType)
    {
    }

    eastl::string_view InputDevice::GetDeviceName() const
    {
        return m_deviceName;
    }

    eastl::string InputDevice::ToString() const
    {
        return eastl::string(m_deviceName);
    }

    DeviceType InputDevice::GetDeviceType() const
    {
        return m_deviceType;
    }
}
