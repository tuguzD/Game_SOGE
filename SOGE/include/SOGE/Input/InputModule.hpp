#ifndef SOGE_INPUT_INPUTMODULE_HPP
#define SOGE_INPUT_INPUTMODULE_HPP

#include "SOGE/Event/EventModule.hpp"
#include "SOGE/System/Memory.hpp"


namespace soge
{
    class InputCore;

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
    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, InputModule)

#endif // SOGE_INPUT_INPUTMODULE_HPP
