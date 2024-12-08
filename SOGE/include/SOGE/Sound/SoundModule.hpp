#ifndef SOGE_SOUND_SOUNDMODULE_HPP
#define SOGE_SOUND_SOUNDMODULE_HPP

#include "SOGE/Event/EventModule.hpp"
#include "SOGE/System/Memory.hpp"


namespace soge
{
    class SoundModule : public Module
    {
    private:
        void Update() const;

        EventModule* m_eventModule;
        EventModule::FunctionHandle m_updateEventHandle;

    public:
        explicit SoundModule();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;

    };
}

#endif // !SOGE_SOUND_SOUNDMODULE_HPP
