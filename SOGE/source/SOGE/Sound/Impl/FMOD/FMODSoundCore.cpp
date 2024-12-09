#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundCore.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODException.hpp"

#ifdef SOGE_WINDOWS
#include <combaseapi.h>
#endif


namespace soge
{
    FMODSoundCore::FMODSoundCore(EventModule* aEventModule)
        : SoundCore(aEventModule), m_fmodStudioSystem(nullptr), m_fmodSystem(nullptr)
    {
        #ifdef SOGE_WINDOWS
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        #endif

        FMOD::Studio::System::create(&m_fmodStudioSystem);
        m_fmodStudioSystem->getCoreSystem(&m_fmodSystem);
        m_fmodSystem->setSoftwareFormat(m_config.AUDIO_SAMPLE_RATE, FMOD_SPEAKERMODE_STEREO, 0);
        m_fmodSystem->set3DSettings(1.0f, m_config.DISTANCE_FACTOR, 0.5f);

        m_fmodStudioSystem->initialize(m_config.MAX_AUDIO_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
        m_fmodSystem->getMasterChannelGroup(&m_masterGroup);
    }

    FMODSoundCore::~FMODSoundCore()
    {
        m_fmodSystem->close();
        m_fmodStudioSystem->release();
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
            FMODThrowIfFailed(sound->set3DMinMaxDistance(0.5f * m_config.DISTANCE_FACTOR, 5000.0f * m_config.DISTANCE_FACTOR));

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
