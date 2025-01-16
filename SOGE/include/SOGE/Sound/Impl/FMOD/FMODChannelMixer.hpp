#ifndef SOGE_SOUND_IMPL_FMOD_FMODCHANNELMIXER_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODCHANNELMIXER_HPP

#include "SOGE/Sound/Public/ChannelMixer.hpp"
#include "SOGE/System/Memory.hpp"
#include <FMOD/fmod.hpp>


namespace soge
{
    class FMODSystem;
    class FMODChannelMixer final : public ChannelMixer
    {
    private:
        static SharedPtr<FMODChannelMixer> s_instance;
        FMODSystem* m_fmodSystem;
        FMOD::ChannelGroup* m_masterGroup;
        eastl::unordered_map<eastl::string_view, FMOD::ChannelGroup*> m_channelGroups;


    protected:
        FMODChannelMixer();


    public:
        FMODChannelMixer(FMODChannelMixer&)     = delete;
        FMODChannelMixer(FMODChannelMixer&&)    = delete;

        auto operator= (FMODChannelMixer&)  = delete;
        auto operator= (FMODChannelMixer&&) = delete;
        ~FMODChannelMixer();

        void CreateChannel(const eastl::string_view& aChannelName) override;
        void PlayOnChannel(const eastl::string_view& aChannelName, SoundResource* aSoundResource) override;

        void StopChannel(const eastl::string_view& aChannelName) override;
        void MuteChannel(const eastl::string_view& aChannelName) override;
        void PauseChannel(const eastl::string_view& aChannelName) override;
        void SetChannelVolume(const eastl::string_view& aChannelName, float aVolume) override;

        bool IsChannelExists(const eastl::string_view& aChannelName) override;

    public:
        static FMODChannelMixer* GetInstance();

    };
}

#endif // !SOGE_SOUND_IMPL_FMOD_FMODCHANNELMIXER_HPP
