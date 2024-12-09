#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundCore.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODException.hpp"


namespace soge
{
    FMODSoundCore::FMODSoundCore(EventModule* aEventModule) : SoundCore(aEventModule)
    {
        FMOD::Studio::System::create(&m_fmodStudioSystem);
        FMODThrowIfFailed(m_fmodStudioSystem->getCoreSystem(&m_fmodSystem));
        FMODThrowIfFailed(m_fmodSystem->setSoftwareFormat(m_maxAudioChannels, FMOD_SPEAKERMODE_STEREO, 0));
        FMODThrowIfFailed(m_fmodSystem->set3DSettings(1.0f, m_distanceFactor, 0.5f));
        FMODThrowIfFailed(m_fmodSystem->getMasterChannelGroup(&m_masterGroup));
        FMODThrowIfFailed(m_fmodStudioSystem->initialize(m_maxAudioChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
    }

    FMODSoundCore::~FMODSoundCore()
    {
    }

    void FMODSoundCore::BeginUpdateSound()
    {
        if (m_isPauseUpdateRequested)
            return;

        // Also updates m_fmodSystem
        m_fmodStudioSystem->update();
    }

    void FMODSoundCore::EndUpdateSound()
    {
        m_isEndUpdateRequested = true;
    }

    void FMODSoundCore::SetPauseUpdate(float aPauseRequested)
    {
        m_isPauseUpdateRequested = aPauseRequested;
    }

    void FMODSoundCore::Set3DListenerPosition(float aPosX,      float aPosY,    float aPosZ,
                                              float aForwardX,  float aForwardY,float aForwardZ,
                                              float aUpX,       float aUpY,     float aUpZ)
    {

    }

    void FMODSoundCore::LoadSoundResource(const SoundResource& aSoundResource)
    {
        if (!aSoundResource.IsLoaded())
        {
            FMOD::Sound* sound = nullptr;

            FMOD_MODE _3dMode = aSoundResource.Is3D() ? FMOD_3D : FMOD_2D;
            FMOD_MODE loopFlag = aSoundResource.IsLoop() ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

            FMODThrowIfFailed(m_fmodSystem->createSound(aSoundResource.GetFullPath().c_str(), _3dMode, nullptr, &sound));
            FMODThrowIfFailed(sound->setMode(loopFlag));
            FMODThrowIfFailed(sound->set3DMinMaxDistance(0.5f * m_distanceFactor, 5000.0f * m_distanceFactor));

        }
    }

    void FMODSoundCore::UnloadSoundResource(const SoundResource& aSoundResource)
    {

    }

    void FMODSoundCore::ReloadSoundResource(const SoundResource& aSoundResource)
    {

    }

    void FMODSoundCore::PlaySoundResource(const SoundResource& aSoundResource)
    {
    }

    void FMODSoundCore::StopSoundResource(const SoundResource& aSoundResource)
    {
    }

    bool FMODSoundCore::IsSoundPlaying(const SoundResource& aSoundResource)
    {
        return false;
    }

    void FMODSoundCore::SetVolume(float aVolume)
    {
    }

    float FMODSoundCore::GetVolume() const
    {
        return 0.0f;
    }
}
