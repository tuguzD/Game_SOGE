#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundChannel.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODException.hpp"


namespace soge
{
    FMODSoundChannel::FMODSoundChannel() : m_fmodSoundChannel(nullptr)
    {
    }

    FMODSoundChannel::~FMODSoundChannel()
    {
        m_fmodSoundChannel->stop();
        delete m_fmodSoundChannel;
    }

    void FMODSoundChannel::SetSoundToChannel(FMOD::System* aFMODSystem, FMODSound* aFMODSound)
    {
        // Just set sound to FMOD channel paused on init
        FMODThrowIfFailed(aFMODSystem->playSound(aFMODSound->GetSound(), nullptr, true, &m_fmodSoundChannel));
        m_isBusy = true;
    }

    void FMODSoundChannel::BeginChannelPlayback()
    {
        FMODThrowIfFailed(m_fmodSoundChannel->setPaused(false));
    }

    void FMODSoundChannel::PauseChannelPlayback()
    {
        FMODThrowIfFailed(m_fmodSoundChannel->setPaused(true));
    }

    void FMODSoundChannel::StopChannelPlayback()
    {
        FMODThrowIfFailed(m_fmodSoundChannel->stop());
    }

    void FMODSoundChannel::ClearSoundChannel()
    {
    }

    bool FMODSoundChannel::IsChannelPlaying() const
    {
        bool isPlaying = false;
        m_fmodSoundChannel->isPlaying(&isPlaying);
        return isPlaying;
    }

    bool FMODSoundChannel::IsChannelBusy() const
    {
        return m_isBusy;
    }

    FMOD::Channel* FMODSoundChannel::GetChannel() const
    {
        return m_fmodSoundChannel;
    }
}
