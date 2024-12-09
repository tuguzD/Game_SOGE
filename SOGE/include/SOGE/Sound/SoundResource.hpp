#ifndef SOGE_SOUND_SOUNDRESOURCE_HPP
#define SOGE_SOUND_SOUNDRESOURCE_HPP

#include "SOGE/Content/ResourceBase.hpp"
#include <cppfs/FilePath.h>


namespace soge
{
    class SoundResource : public ResourceBase
    {
    protected:
        unsigned int m_length;
        bool m_loop;
        bool m_is3d;

    public:
        SoundResource(const eastl::string& aName, const cppfs::FilePath& aFullPath);
        ~SoundResource() = default;

        bool Reload()  override;
        void Unload()  override;
        void Destroy() override;

        void SetLoop(bool aLoop);
        void Set3D(bool aNeed3D);

        bool IsLoop() const;
        bool Is3D() const;

    };
}

#endif // !SOGE_SOUND_SOUNDRESOURCE_HPP
