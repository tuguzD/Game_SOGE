#ifndef SOGE_INPUT_MOUSE_HPP
#define SOGE_INPUT_MOUSE_HPP

#include "SOGE/Input/InputDevice.hpp"


namespace soge
{
    class Mouse : public InputDevice
    {
    public:
        Mouse(eastl::string_view aMouseName);
        virtual ~Mouse() = default;

        virtual void Update() = 0;

        virtual bool IsButtonPressed() = 0;
        virtual bool IsButtonReleased() = 0;

    };
}

#endif // !SOGE_INPUT_MOUSE_HPP
