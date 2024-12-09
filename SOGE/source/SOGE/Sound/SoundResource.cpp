#include "sogepch.hpp"
#include "SOGE/Sound/SoundResource.hpp"


namespace soge
{
    SoundResource::SoundResource(const eastl::string& aName, const cppfs::FilePath& aFullPath) : ResourceBase(aName, aFullPath)
    {
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

    bool SoundResource::IsLoop() const
    {
        return m_loop;
    }

    bool SoundResource::Is3D() const
    {
        return m_is3d;
    }
}
