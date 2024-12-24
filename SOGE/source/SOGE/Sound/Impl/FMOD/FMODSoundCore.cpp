#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundCore.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODException.hpp"


namespace soge
{
    FMODSoundCore::FMODSoundCore(EventModule& aEventModule) : SoundCore(aEventModule)
    {
        SOGE_INFO_LOG("Initialize FMOD...");

        m_config.m_maxChannelCount = 255;
        m_config.m_distanceFactor = 1.0f;

        FMODThrowIfFailed(FMOD::Studio::System::create(&m_fmodStudioSystem));
        FMODThrowIfFailed(m_fmodStudioSystem->getCoreSystem(&m_fmodSystem));
        FMODThrowIfFailed(m_fmodStudioSystem->initialize(255, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
        m_defaultSoundChannel = new FMODSoundChannel();
    }

    FMODSoundCore::~FMODSoundCore()
    {
        FMODThrowIfFailed(m_fmodSystem->close());
        FMODThrowIfFailed(m_fmodStudioSystem->release());
    }

    void FMODSoundCore::Update()
    {
        FMODThrowIfFailed(m_fmodStudioSystem->update());
    }

    void FMODSoundCore::Update3DListener(const glm::vec3& aPos,
                                         const glm::vec3& aForwardVec,
                                         const glm::vec3& aUpwardVec)
    {
        FMOD_VECTOR pos     = { aPos.x, aPos.y, aPos.z };
        FMOD_VECTOR forward = { aForwardVec.x, aForwardVec.y, aForwardVec.z };
        FMOD_VECTOR upward  = { aUpwardVec.x, aUpwardVec.y, aUpwardVec.z };
        m_listener.Set3DListenerPosition(pos, forward, upward);
    }

    void FMODSoundCore::LoadSoundResource(SoundResource& aSoundResource)
    {
        FMODSound* sound = new FMODSound(aSoundResource);
        if (sound->Load(m_fmodSystem, m_config))
        {
            m_loadedSounds.insert({aSoundResource.GetUUID(), sound});
        }
        aSoundResource.SetLoaded(true);
    }

    void FMODSoundCore::PlaySoundResource(SoundResource& aSoundResource)
    {
        FMODSound* sound = m_loadedSounds[aSoundResource.GetUUID()];
        m_defaultSoundChannel->SetSoundToChannel(m_fmodSystem, sound);
        m_defaultSoundChannel->BeginChannelPlayback();
        aSoundResource.SetPlaying(true);
    }

    void FMODSoundCore::PauseSoundResource(SoundResource& aSoundResource)
    {
        m_defaultSoundChannel->PauseChannelPlayback();
        aSoundResource.SetPaused(true);
    }

    void FMODSoundCore::UnpauseSoundResource(SoundResource& aSoundResource)
    {
        aSoundResource.SetPaused(false);
    }

    void FMODSoundCore::StopSoundResource(SoundResource& aSoundResource)
    {
        if (aSoundResource.IsPlaying())
        {
            m_defaultSoundChannel->StopChannelPlayback();
            aSoundResource.SetStopped(true);
        }
    }

    bool FMODSoundCore::IsSoundResourcePlaying(SoundResource& aSoundResource)
    {
        return false;
    }
}
