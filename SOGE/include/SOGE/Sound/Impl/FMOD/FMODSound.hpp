#ifndef SOGE_SOUND_IMPL_FMOD_FMODSOUND_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODSOUND_HPP

#include "SOGE/Sound/SoundResource.hpp"
#include <FMOD/fmod.hpp>


namespace soge
{
    class FMODSound
    {
    private:
        FMOD::Sound* m_fmodSound;
        SoundResource& m_soundResource;

    public:
        FMODSound(SoundResource& aSoundResource);
        FMODSound(FMOD::Sound* aFMODSound, SoundResource& aSoundResource);
        ~FMODSound();

        bool Load(FMOD::System* aFMODSystem);
        bool Reload(FMOD::System* aFMODSystem);
        bool Unload(FMOD::System* aFMODSystem);
        FMOD::Sound* GetSound() const;

    };
}

#endif // !SOGE_SOUND_IMPL_FMOD_FMODSOUND_HPP
