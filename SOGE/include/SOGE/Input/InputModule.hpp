#ifndef SOGE_INPUT_INPUTMODULE_HPP
#define SOGE_INPUT_INPUTMODULE_HPP

#include "SOGE/Core/Module.hpp"
#include "SOGE/System/Memory.hpp"


namespace soge
{
    class Mouse;
    class Keyboard;
    class Gamepad;
    class InputDevice;
    class InputCore;

    class InputModule : public Module
    {
    private:
        SharedPtr<Keyboard> m_keyboard;
        SharedPtr<Mouse> m_mouse;

        eastl::list<SharedPtr<Gamepad>> m_gamepadList;
        eastl::list<SharedPtr<InputDevice>> m_deviceList;
        SharedPtr<InputCore> m_inputCore;

    public:
        InputModule();

        void Update();

        void Load(di::Container& aContainer) override;
        void Unload(di::Container& aContainer) override;
    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, InputModule)

#endif // SOGE_INPUT_INPUTMODULE_HPP
