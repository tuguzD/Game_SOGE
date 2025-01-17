#ifndef SOGE_SOUND_PUBLIC_CHANNELMIXER_HPP
#define SOGE_SOUND_PUBLIC_CHANNELMIXER_HPP

#include <EASTL/string.h>


namespace soge
{
    class SoundResource;
    class ChannelMixer
    {
    public:
        constexpr virtual ~ChannelMixer() noexcept = default;


        virtual void CreateChannel(const eastl::string_view& aChannelName) = 0;
        virtual void PlayOnChannel(const eastl::string_view& aChannelName, SoundResource* aSoundResource) = 0;

        virtual void StopChannel(const eastl::string_view& aChannelName) = 0;
        virtual void MuteChannel(const eastl::string_view& aChannelName) = 0;
        virtual void PauseChannel(const eastl::string_view& aChannelName) = 0;
        virtual void SetChannelVolume(const eastl::string_view& aChannelName, float aVolume) = 0;

        virtual bool IsChannelExists(const eastl::string_view& aChannelName) = 0;

    };
}

#endif // !SOGE_SOUND_PUBLIC_CHANNELMIXER_HPP
