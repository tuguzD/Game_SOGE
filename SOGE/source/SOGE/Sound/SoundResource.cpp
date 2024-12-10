#include "sogepch.hpp"
#include "SOGE/Sound/SoundResource.hpp"


namespace soge
{
    SoundResource::SoundResource(const eastl::string& aName, const cppfs::FilePath& aFullPath)
        : ResourceBase(aName, aFullPath), m_actualLengthMs(0)
    {
    }

    void SoundResource::SetActualLengthMs(unsigned int aLengthMs)
    {
        m_actualLengthMs = aLengthMs;
        m_userLengthMs = aLengthMs;
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

    void SoundResource::SetLoop(bool aLoop)
    {
        m_loop = aLoop;
    }

    void SoundResource::Set3D(bool aNeed3D)
    {
        m_is3d = aNeed3D;
    }

    void SoundResource::SetLength(unsigned int aLengthMs)
    {
        if (aLengthMs > m_actualLengthMs)
        {
            SOGE_WARN_LOG("Given audio length is more than actual length; {0} > {1}", aLengthMs, m_actualLengthMs);
            return;
        }
        m_userLengthMs = aLengthMs;
    }

    void SoundResource::SetVolume(float aVolume)
    {
        m_volume = aVolume;
    }

    float SoundResource::GetVolume() const
    {
        return m_volume;
    }

    float SoundResource::GetReverbAmount() const
    {
        return m_reverbAmount;
    }

    bool SoundResource::IsLoop() const
    {
        return m_loop;
    }

    bool SoundResource::Is3D() const
    {
        return m_is3d;
    }

    unsigned int SoundResource::GetActualLengthMs() const
    {
        return m_actualLengthMs;
    }

    unsigned int SoundResource::GetUserLengthMs() const
    {
        return m_userLengthMs;
    }

    FriendFuncPtr<ResourceBase, void, bool> SoundResource::FriendlySetLoadState()
    {
        return FriendFuncPtr(&ResourceBase::SetLoaded);
    }

    FriendFuncPtr<SoundResource, void, unsigned int> SoundResource::FriendlySetInitialLength()
    {
        return FriendFuncPtr(&SoundResource::SetActualLengthMs);
    }

}
