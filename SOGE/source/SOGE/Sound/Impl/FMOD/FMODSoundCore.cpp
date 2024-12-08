#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundCore.hpp"


namespace soge
{
    FMODSoundCore::FMODSoundCore(EventModule* aEventModule) : SoundCore(aEventModule)
    {
        FMOD_RESULT fresult;
        FMOD::System* fmodSystem;
        fresult = FMOD::System_Create(&fmodSystem);
        m_fmodSystem = SharedPtr<FMOD::System>(fmodSystem);

    }

    void FMODSoundCore::BeginUpdateSound()
    {
    }

    void FMODSoundCore::EndUpdateSound()
    {

    }

    void FMODSoundCore::SetPauseUpdate()
    {
    }

    void FMODSoundCore::SetVolume(float aVolume)
    {
    }
}
