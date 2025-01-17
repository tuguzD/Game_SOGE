#include "sogepch.hpp"
#include "SOGE/Sound/SoundResource.hpp"


namespace soge
{
    SoundResource::SoundResource(const eastl::string_view& aName, const cppfs::FilePath& aFullPath)
        : ResourceBase(aName, aFullPath)
    {
        m_is3d      = true;
        m_isLooping = false;
        m_isPaused  = false;
        m_isPlaying = false;
        m_isStopped = true;
    }

    void SoundResource::SetActualLengthMs(unsigned int aLengthMs)
    {
    }

    void SoundResource::SetPlaying(bool aPlay) noexcept
    {
        m_isPlaying = aPlay;
    }

    void SoundResource::SetStopped(bool aStop) noexcept
    {
        m_isStopped = aStop;
    }

    void SoundResource::SetPaused(bool aPause) noexcept
    {
        m_isPaused = aPause;
    }

    bool SoundResource::Reload()
    {
        return false;
    }

    void SoundResource::Unload()
    {
    }

    void SoundResource::Destroy()
    {
    }

    void SoundResource::Enable3D(bool aEnable3D) noexcept
    {
        m_is3d = aEnable3D;
    }

    void SoundResource::EnableLooping(bool aEnableLoop) noexcept
    {
        m_isLooping = aEnableLoop;
    }

    bool SoundResource::IsPlaying() const noexcept
    {
        return m_isPlaying;
    }

    bool SoundResource::IsPaused() const noexcept
    {
        return m_isPaused;
    }

    bool SoundResource::IsStopped() const noexcept
    {
        return m_isStopped;
    }

    bool SoundResource::IsLooping() const noexcept
    {
        return m_isLooping;
    }

    bool SoundResource::Is3D() const noexcept
    {
        return m_is3d;
    }

}
