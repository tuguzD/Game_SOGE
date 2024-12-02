#ifndef SOGE_INPUT_MOUSE_HPP
#define SOGE_INPUT_MOUSE_HPP

#include "SOGE/Input/Device/InputDevice.hpp"
#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    class Mouse : public InputDevice
    {
    public:
        Mouse(eastl::string_view aMouseName);
        virtual ~Mouse() = default;

        virtual void Update() = 0;
        virtual bool IsButtonPressed(const Key aMouseButton) = 0;
        virtual bool IsButtonReleased(const Key aMouseButton) = 0;

    };
}

#endif // !SOGE_INPUT_MOUSE_HPP
