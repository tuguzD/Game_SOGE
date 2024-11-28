#ifndef SOGE_INPUT_GAMEPAD_HPP
#define SOGE_INPUT_GAMEPAD_HPP

#include "SOGE/Input/InputDevice.hpp"


namespace soge
{
    class Gamepad : public InputDevice
    {
    public:
        Gamepad(eastl::string_view aGamepadName);
        virtual ~Gamepad() = default;

        // TODO: Add methods for gamepad...
    };
}

#endif // !SOGE_INPUT_GAMEPAD_HPP
