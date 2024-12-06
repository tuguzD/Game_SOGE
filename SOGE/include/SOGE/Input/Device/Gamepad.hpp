#ifndef SOGE_INPUT_DEVICE_GAMEPAD_HPP
#define SOGE_INPUT_DEVICE_GAMEPAD_HPP

#include "SOGE/Input/Device/InputDevice.hpp"


namespace soge
{
    class Gamepad : public InputDevice
    {
    public:
        explicit Gamepad(eastl::string_view aGamepadName);

        // TODO: Add methods for gamepad...
    };
}

#endif // SOGE_INPUT_DEVICE_GAMEPAD_HPP
