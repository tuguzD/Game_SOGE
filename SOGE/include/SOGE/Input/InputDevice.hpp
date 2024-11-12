#ifndef SOGE_INPUT_INPUTDEVICE_HPP
#define SOGE_INPUT_INPUTDEVICE_HPP

#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    class InputDevice
    {
    private:
        eastl::string m_deviceName;
        InputDeviceType m_deviceType;

    public:
        InputDevice(eastl::string aName, InputDeviceType aType);
        virtual ~InputDevice() = default;

        eastl::string GetName();
        InputDeviceType GetDeviceType();

    };
}

#endif // !SOGE_INPUT_INPUTDEVICE_HPP
