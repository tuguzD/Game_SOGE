#ifndef SOGE_INPUT_INPUTMANAGER_HPP
#define SOGE_INPUT_INPUTMANAGER_HPP

#include "SOGE/System/Memory.hpp"


namespace soge
{
    class Mouse;
    class Keyboard;
    class Gamepad;
    class InputDevice;
    class InputCore;

    class InputManager
    {
    private:
        SharedPtr<Keyboard> m_keyboard;
        SharedPtr<Mouse> m_mouse;

        eastl::list<SharedPtr<Gamepad>> m_gamepadList;
        eastl::list<SharedPtr<InputDevice>> m_deviceList;
        SharedPtr<InputCore> m_inputCore;

    public:
        InputManager();
        ~InputManager() = default;

        void Update();
    };
}

#endif // SOGE_INPUT_INPUTMANAGER_HPP
