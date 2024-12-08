#ifndef SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP

#include "SOGE/Sound/SoundCore.hpp"
#include "SOGE/System/Memory.hpp"

#include <FMOD/fmod.hpp>


namespace soge
{
    class FMODSoundCore : public SoundCore
    {
    private:
        SharedPtr<FMOD::System> m_fmodSystem;

    public:
        explicit FMODSoundCore(EventModule* aEventModule);

        virtual void BeginUpdateSound() override;
        virtual void EndUpdateSound() override;
        virtual void SetPauseUpdate() override;

        void SetVolume(float aVolume) override;

    };

    using ImplSoundCore = FMODSoundCore;
}

#endif // SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP
