#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODChannelMixer.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundCore.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODException.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundChannel.hpp"


namespace soge
{
    SharedPtr<FMODChannelMixer> FMODChannelMixer::s_instance = nullptr;
    FMODChannelMixer* FMODChannelMixer::GetInstance()
    {
        if (s_instance == nullptr)
        {
            s_instance = SharedPtr<FMODChannelMixer>(new FMODChannelMixer());
        }

        return s_instance.get();
    }

    FMODChannelMixer::FMODChannelMixer()
    {
        m_fmodSystem = FMODSystem::GetInstance();
        FMODThrowIfFailed(m_fmodSystem->GetSystem()->getMasterChannelGroup(&m_masterGroup));
    }

    FMODChannelMixer::~FMODChannelMixer()
    {
        auto it = m_channelGroups.begin();
        for (; it != m_channelGroups.end(); ++it)
        {
            it->second->release();
        }
    }

    void FMODChannelMixer::CreateChannel(const eastl::string_view& aChannelName)
    {
        assert(IsChannelExists(aChannelName) != true);

        FMOD::System* fmodSystem = FMODSystem::GetInstance()->GetSystem();
        FMOD::ChannelGroup* channelGroup = nullptr;

        FMODThrowIfFailed(fmodSystem->createChannelGroup(aChannelName.data(), &channelGroup));
        FMODThrowIfFailed(m_masterGroup->addGroup(channelGroup));
        m_channelGroups.insert({aChannelName, channelGroup});
    }

    void FMODChannelMixer::PlayOnChannel(const eastl::string_view& aChannelName, SoundResource* aSoundResource)
    {
        FMOD::System* fmodSystem            = m_fmodSystem->GetSystem();
        FMOD::Channel* channel              = nullptr;
        FMOD::Sound* sound                  = m_fmodSystem->m_loadedSounds[aSoundResource->GetUUID()]->GetSound();
        FMOD::ChannelGroup* channelGroup    = m_channelGroups.at(aChannelName);

        FMODThrowIfFailed(fmodSystem->playSound(sound, channelGroup, false, &channel));
    }

    void FMODChannelMixer::StopChannel(const eastl::string_view& aChannelName)
    {
        FMOD::ChannelGroup* channelGroup = m_channelGroups.at(aChannelName);
        channelGroup->stop();
    }

    void FMODChannelMixer::MuteChannel(const eastl::string_view& aChannelName)
    {
        FMOD::ChannelGroup* channelGroup = m_channelGroups.at(aChannelName);
        channelGroup->setMute(true);
    }

    void FMODChannelMixer::PauseChannel(const eastl::string_view& aChannelName)
    {
        FMOD::ChannelGroup* channelGroup = m_channelGroups.at(aChannelName);
        channelGroup->setPaused(true);
    }

    void FMODChannelMixer::SetChannelVolume(const eastl::string_view& aChannelName, float aVolume)
    {
        FMOD::ChannelGroup* channelGroup = m_channelGroups.at(aChannelName);
        channelGroup->setVolume(aVolume);
    }

    bool FMODChannelMixer::IsChannelExists(const eastl::string_view& aChannelName)
    {
        return m_channelGroups.find(aChannelName) != m_channelGroups.end();
    }
}
