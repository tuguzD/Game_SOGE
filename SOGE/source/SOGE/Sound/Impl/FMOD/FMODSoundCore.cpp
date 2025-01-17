#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundCore.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODException.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODChannelMixer.hpp"

#include "SOGE/Sound/Public/ChannelMixer.hpp"
#include "SOGE/Sound/Public/Listener.hpp"


namespace soge
{
    FMODSoundCore::FMODSoundCore(EventModule& aEventModule) : SoundCore(aEventModule)
    {
        SOGE_INFO_LOG("Initialize FMOD...");

        m_config.m_maxChannelCount = 255;
        m_config.m_distanceFactor = 1.0f;

        m_listener = SharedPtr<Listener>(new Listener());
        m_fmodSystem = FMODSystem::GetInstance();
        m_fmodSystem->Initialize();

        m_fmodSystem->m_defaultSoundChannel = new FMODSoundChannel();
    }

    FMODSoundCore::~FMODSoundCore()
    {
        m_fmodSystem->Destroy();
    }

    void FMODSoundCore::Update()
    {
        FMODThrowIfFailed(m_fmodSystem->GetStudioSystem()->update());
    }

    void FMODSoundCore::Update3DListener(const glm::vec3& aPos,
                                         const glm::vec3& aForwardVec,
                                         const glm::vec3& aUpwardVec)
    {
        m_listener->Update(aPos, aForwardVec, aUpwardVec);
    }

    ChannelMixer* FMODSoundCore::GetChannelMixer()
    {
        return FMODChannelMixer::GetInstance();
    }

    SoundResource* FMODSoundCore::CreateSoundResource(const eastl::string_view& aName, const cppfs::FilePath& aFilePath,
                                                      bool aIs3D)
    {
        SoundResource* resource = new SoundResource(aName, aFilePath);
        resource->Enable3D(aIs3D);
        return resource;
    }

    void FMODSoundCore::LoadSoundResource(SoundResource* aSoundResource)
    {
        FMODSound* sound = new FMODSound(aSoundResource);
        if (sound->Load(m_config))
        {
            m_fmodSystem->m_loadedSounds.insert({aSoundResource->GetUUID(), sound});
        }
        aSoundResource->SetLoaded(true);
    }

    void FMODSoundCore::PlaySoundResource(SoundResource* aSoundResource)
    {
        FMODSound* sound = m_fmodSystem->m_loadedSounds[aSoundResource->GetUUID()];
        m_fmodSystem->m_defaultSoundChannel->SetSoundToChannel(m_fmodSystem->GetSystem(), sound);
        m_fmodSystem->m_defaultSoundChannel->BeginChannelPlayback();
        aSoundResource->SetPlaying(true);
    }

    void FMODSoundCore::PauseSoundResource(SoundResource* aSoundResource)
    {
        m_fmodSystem->m_defaultSoundChannel->PauseChannelPlayback();
        aSoundResource->SetPaused(true);
    }

    void FMODSoundCore::UnpauseSoundResource(SoundResource* aSoundResource)
    {
        aSoundResource->SetPaused(false);
    }

    void FMODSoundCore::StopSoundResource(SoundResource* aSoundResource)
    {
        if (aSoundResource->IsPlaying())
        {
            m_fmodSystem->m_defaultSoundChannel->StopChannelPlayback();
            aSoundResource->SetStopped(true);
        }
    }

    bool FMODSoundCore::IsSoundResourcePlaying(SoundResource* aSoundResource)
    {
        return false;
    }

    ////////////////
    // FMODSystem
    ////////////////

    SharedPtr<FMODSystem> FMODSystem::s_instance = nullptr;
    FMODSystem* FMODSystem::GetInstance()
    {
        if (s_instance == nullptr)
        {
            s_instance = SharedPtr<FMODSystem>(new FMODSystem());
        }

        return s_instance.get();
    }

    FMODSystem::FMODSystem()
    {
    }

    FMODSystem::~FMODSystem()
    {
        Destroy();
    }

    void FMODSystem::Initialize()
    {
        FMODThrowIfFailed(m_fmodStudioSystem->create(&m_fmodStudioSystem));
        FMODThrowIfFailed(m_fmodStudioSystem->getCoreSystem(&m_fmodSystem));
        FMODThrowIfFailed(m_fmodStudioSystem->initialize(255, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_3D_RIGHTHANDED, nullptr));
    }

    void FMODSystem::Destroy()
    {
        m_fmodSystem->close();
        m_fmodStudioSystem->release();
    }

    FMOD::Studio::System* FMODSystem::GetStudioSystem() const
    {
        return m_fmodStudioSystem;
    }

    FMOD::System* FMODSystem::GetSystem() const
    {
        return m_fmodSystem;
    }
}
