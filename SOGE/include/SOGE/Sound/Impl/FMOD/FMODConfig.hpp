#ifndef SOGE_SOUND_IMPL_FMOD_FMODCONFIG_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODCONFIG_HPP


namespace soge
{
    class FMODConfig
    {
        friend class FMODSoundCore;

    protected:
        unsigned int m_maxChannelCount;
        float m_distanceFactor;

    public:
        FMODConfig() = default;
        ~FMODConfig() = default;

        unsigned int GetMaxChannelCount() const;
        float GetDistanceFactor() const;

    };
}

#endif // !SOGE_SOUND_IMPL_FMOD_FMODCONFIG_HPP
