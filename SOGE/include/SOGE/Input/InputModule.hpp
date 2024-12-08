#ifndef SOGE_INPUT_INPUTMODULE_HPP
#define SOGE_INPUT_INPUTMODULE_HPP

#include "SOGE/Event/EventModule.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Input/InputCore.hpp"
#include "SOGE/Input/Device/Keyboard.hpp"
#include "SOGE/Input/Device/Gamepad.hpp"
#include "SOGE/Input/Device/Mouse.hpp"


namespace soge
{
    class InputModule : public Module
    {
    private:
        void Update() const;

        EventModule* m_eventModule;
        EventModule::FunctionHandle m_updateEventHandle;

        UniquePtr<InputCore> m_inputCore;

    public:
        explicit InputModule();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;

        bool IsKeyPressed(const Key& aKey) const;
        bool IsKeyReleased(const Key& aKey) const;

        Keyboard* GetKeyboard() const;
        Gamepad* GetGamepad() const;
        Mouse* GetMouse() const;

    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, InputModule)

#endif // SOGE_INPUT_INPUTMODULE_HPP
