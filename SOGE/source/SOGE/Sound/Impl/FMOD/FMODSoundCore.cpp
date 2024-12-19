#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundCore.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODException.hpp"


namespace soge
{
    FMODSoundCore::FMODSoundCore(EventModule& aEventModule) : SoundCore(aEventModule)
    {
        SOGE_INFO_LOG("Initialize FMOD...");

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

    void FMODSoundCore::LoadSoundResource(SoundResource& aSoundResource)
    {
        FMODSound* sound = new FMODSound(aSoundResource);
        if (sound->Load(m_fmodSystem))
        {
            m_loadedSounds.insert({aSoundResource.GetUUID(), sound});
        }
    }

    void FMODSoundCore::PlaySoundResource(SoundResource& aSoundResource)
    {
        FMODSound* sound = m_loadedSounds[aSoundResource.GetUUID()];
        m_defaultSoundChannel->SetSoundToChannel(m_fmodSystem, sound);
        m_defaultSoundChannel->BeginChannelPlayback();
    }

    bool FMODSoundCore::IsSoundResourcePlaying(SoundResource& aSoundResource)
    {
        return false;
    }
}
