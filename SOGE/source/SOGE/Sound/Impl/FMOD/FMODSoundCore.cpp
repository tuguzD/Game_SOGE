#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundCore.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODException.hpp"
#include "SOGE/Containers/FriendFuncPtr.hpp"

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

        m_listenerHeadPosition  = {0.0f, 0.0f, -1.0f * m_config.DISTANCE_FACTOR};
        m_listenerForwardVector = {0.0f, 0.0f, 1.0f};
        m_listenerUpVector      = {0.0f, 1.0f, 0.0f};

        FMODThrowIfFailed(FMOD::Studio::System::create(&m_fmodStudioSystem));
        FMODThrowIfFailed(m_fmodStudioSystem->getCoreSystem(&m_fmodSystem));
        FMODThrowIfFailed(m_fmodSystem->setSoftwareFormat(m_config.AUDIO_SAMPLE_RATE, FMOD_SPEAKERMODE_STEREO, 0));
        FMODThrowIfFailed(m_fmodSystem->set3DSettings(1.0f, m_config.DISTANCE_FACTOR, 0.5f));

        FMODThrowIfFailed(m_fmodStudioSystem->initialize(m_config.MAX_AUDIO_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
        FMODThrowIfFailed(m_fmodSystem->getMasterChannelGroup(&m_masterGroup));
        InitReverb();
    }

    FMODSoundCore::~FMODSoundCore()
    {
        FMODThrowIfFailed(m_fmodSystem->close());
        FMODThrowIfFailed(m_fmodStudioSystem->release());
    }

    void FMODSoundCore::InitReverb()
    {
        FMOD_REVERB_PROPERTIES reverbProp = FMOD_PRESET_CONCERTHALL;
        FMODThrowIfFailed(m_fmodSystem->createReverb3D(&m_fmodReverb));
        FMODThrowIfFailed(m_fmodReverb->setProperties(&reverbProp));
        FMODThrowIfFailed(m_fmodReverb->set3DAttributes(&m_reverbPosition, m_reverbMinDistance, m_reverbMaxDistance));
    }

    void FMODSoundCore::BeginUpdateSound()
    {
        if (m_isPauseUpdateRequested)
            return;
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

    void FMODSoundCore::LoadSoundResource(SoundResource& aSoundResource)
    {
        if (!aSoundResource.IsLoaded())
        {
            FMOD::Sound* sound = nullptr;
            unsigned int soundLength;

            FMOD_MODE _3dMode = aSoundResource.Is3D() ? FMOD_3D : FMOD_2D;
            FMOD_MODE loopFlag = aSoundResource.IsLoop() ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

            FMODThrowIfFailed(m_fmodSystem->createSound(aSoundResource.GetFullPath().c_str(), _3dMode, nullptr, &sound));
            FMODThrowIfFailed(sound->setMode(loopFlag));
            FMODThrowIfFailed(sound->set3DMinMaxDistance(0.5f * m_config.DISTANCE_FACTOR, 5000.0f * m_config.DISTANCE_FACTOR));

            // Set sound resource length
            sound->getLength(&soundLength, FMOD_TIMEUNIT_MS);
            FriendFuncAccessor actualLenAccessor(SoundResource::FriendlySetInitialLength());
            actualLenAccessor.Call(aSoundResource, soundLength);

            // Set sound resource load state
            m_sounds.insert({aSoundResource.GetUUID(), sound});
            FriendFuncAccessor loadStateAccessor(SoundResource::FriendlySetLoadState());
            actualLenAccessor.Call(aSoundResource, true);
        }
        else
        {
            SOGE_WARN_LOG("File {} was already loaded", aSoundResource.GetName().c_str());
        }
    }

    void FMODSoundCore::UnloadSoundResource(SoundResource& aSoundResource)
    {

    }

    void FMODSoundCore::ReloadSoundResource(SoundResource& aSoundResource)
    {

    }

    void FMODSoundCore::PlaySoundResource(SoundResource& aSoundResource)
    {
    }

    void FMODSoundCore::StopSoundResource(SoundResource& aSoundResource)
    {
    }

    bool FMODSoundCore::IsSoundPlaying(SoundResource& aSoundResource)
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
