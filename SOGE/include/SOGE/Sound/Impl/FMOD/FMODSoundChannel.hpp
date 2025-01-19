#ifndef SOGE_SOUND_IMPL_FMOD_FMODSOUNDCHANNEL_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODSOUNDCHANNEL_HPP

#include "SOGE/Sound/Impl/FMOD/FMODSound.hpp"
#include <FMOD/fmod.hpp>


namespace soge
{
    class FMODSoundChannel
    {
    private:
        FMOD::Channel* m_fmodSoundChannel;
        bool m_isBusy;

    public:
        FMODSoundChannel();
        ~FMODSoundChannel();

        void SetSoundToChannel(FMOD::System* aFMODSystem, FMODSound* aFMODSound);
        void BeginChannelPlayback();
        void PauseChannelPlayback();
        void StopChannelPlayback();
        void ClearSoundChannel();

        bool IsChannelPlaying() const;
        bool IsChannelBusy() const;
        FMOD::Channel* GetChannel() const;

    };
}

#endif // !SOGE_SOUND_IMPL_FMOD_FMODSOUNDCHANNEL_HPP
