#ifndef SOGE_INPUT_DEVICE_MOUSE_HPP
#define SOGE_INPUT_DEVICE_MOUSE_HPP

#include "SOGE/Input/Device/InputDevice.hpp"
#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    class Mouse : public InputDevice
    {
    public:
        explicit Mouse(eastl::string_view aMouseName);

        virtual void Update() = 0;
        virtual bool IsButtonPressed(Key aMouseButton) = 0;
        virtual bool IsButtonReleased(Key aMouseButton) = 0;
    };
}

#endif // SOGE_INPUT_DEVICE_MOUSE_HPP
