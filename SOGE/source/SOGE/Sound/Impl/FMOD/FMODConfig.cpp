#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODConfig.hpp"


namespace soge
{
    unsigned int FMODConfig::GetMaxChannelCount() const
    {
        return m_maxChannelCount;
    }

    float FMODConfig::GetDistanceFactor() const
    {
        return m_distanceFactor;
    }
}
