#ifndef SOGE_SOUND_SOUNDMODULE_HPP
#define SOGE_SOUND_SOUNDMODULE_HPP

#include "SOGE/Event/EventModule.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Sound/SoundCore.hpp"


namespace soge
{
    class SoundModule : public Module
    {
    private:
        void Update() const;

        EventModule* m_eventModule;
        EventModule::FunctionHandle m_updateEventHandle;

        UniquePtr<SoundCore> m_soundCore;

    public:
        explicit SoundModule();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;

    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, SoundModule)

#endif // !SOGE_SOUND_SOUNDMODULE_HPP
