#ifndef SOGE_INPUT_INPUTTYPES_HPP
#define SOGE_INPUT_INPUTTYPES_HPP

#include <EASTL/Utility.h>


namespace soge
{
    struct Key
    {
        Key();
    };

    enum InputDeviceType
    {
        InputDevice_Mouse,
        InputDevice_Keyboard,
        InputDevice_Gamepad
    };

    enum MouseButton
    {
        MouseButton_Left,
        MouseButton_Middle,
        MouseButton_Right,
        MouseButton_WheelUp,
        MouseButton_WheelDown,
        MouseButton_ExtraButton6,
        MouseButton_ExtraButton7,
        MouseButton_ExtraButton8,
        MouseButton_ExtraButton9
    };
}

#endif // !SOGE_INPUT_INPUTTYPES_HPP
