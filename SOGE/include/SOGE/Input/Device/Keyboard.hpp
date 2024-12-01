#ifndef SOGE_INPUT_KEYBOARD_HPP
#define SOGE_INPUT_KEYBOARD_HPP

#include "SOGE/Input/Device/InputDevice.hpp"
#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    class Keyboard : public InputDevice
    {
    public:
        Keyboard(eastl::string_view aKeyboardName);
        virtual ~Keyboard() = default;

        virtual void Update() = 0;

        virtual bool IsKeyPressed(Key aKeyName)     = 0;
        virtual bool IsKeyReleased(Key aKeyName)    = 0;

        virtual Key GetPressedKey()     = 0;
        virtual Key GetReleasedKey()    = 0;

        virtual bool IsAnyKeyPressed()  = 0;
        virtual bool IsAnyKeyReleased() = 0;

    };
}

#endif // !SOGE_INPUT_KEYBOARD_HPP
