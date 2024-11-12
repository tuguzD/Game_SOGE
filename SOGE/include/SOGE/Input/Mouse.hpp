#ifndef SOGE_INPUT_MOUSE_HPP
#define SOGE_INPUT_MOUSE_HPP

#include "SOGE/Input/InputDevice.hpp"
#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    class Mouse : public InputDevice
    {
    public:
        Mouse(eastl::string aName);
        virtual ~Mouse() = default;

        virtual bool IsButtonPressed(MouseButton) = 0;
        virtual eastl::string ButtonToString(MouseButton aButton);
        virtual MouseButton StringToButton(eastl::string aButtonName);

    };
}

#endif // !SOGE_INPUT_MOUSE_HPP
