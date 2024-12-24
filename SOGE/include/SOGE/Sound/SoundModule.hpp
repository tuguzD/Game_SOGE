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
        SoundCore* m_soundCore;

    public:
        explicit SoundModule();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;

        void LoadSoundResource(SoundResource& aSoundResource) const;
        //void UnloadSoundResource(SoundResource& aSoundResource) const;
        //void ReloadSoundResource(SoundResource& aSoundResource) const;

        void PlaySoundResource(SoundResource& aSoundResource) const;
        void PauseSoundResource(SoundResource& aSoundResource) const;
        void StopSoundResource(SoundResource& aSoundResource) const;
        //void StopSoundResource(SoundResource& aSoundResource)

        void Update() const;

    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, SoundModule)

#endif // !SOGE_SOUND_SOUNDMODULE_HPP
