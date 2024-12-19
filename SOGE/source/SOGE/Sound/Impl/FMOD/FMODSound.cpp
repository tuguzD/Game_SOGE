#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSound.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODException.hpp"


namespace soge
{
    FMODSound::FMODSound(SoundResource& aSoundResource) : m_fmodSound(nullptr), m_soundResource(aSoundResource)
    {
    }

    FMODSound::FMODSound(FMOD::Sound* aFMODSound, SoundResource& aSoundResource)
        : m_fmodSound(aFMODSound), m_soundResource(aSoundResource)
    {
    }

    FMODSound::~FMODSound()
    {
        m_fmodSound->release();
    }

    bool FMODSound::Load(FMOD::System* aFMODSystem)
    {
        if (!m_soundResource.IsLoaded())
        {
            FMODThrowIfFailed(
                aFMODSystem->createSound(m_soundResource.GetFullPath().c_str(), FMOD_DEFAULT, nullptr, &m_fmodSound));
            FMODThrowIfFailed(m_fmodSound->setMode(FMOD_LOOP_OFF));
            return true;
        }
        SOGE_WARN_LOG("Sound resource {} already loaded!", m_soundResource.GetName().c_str());
        return false;
    }

    bool FMODSound::Reload(FMOD::System* aFMODSystem)
    {
        return false;
    }

    bool FMODSound::Unload(FMOD::System* aFMODSystem)
    {
        return false;
    }

    FMOD::Sound* FMODSound::GetSound() const
    {
        return m_fmodSound;
    }
}
