#ifndef SOGE_SOUND_SOUNDRESOURCE_HPP
#define SOGE_SOUND_SOUNDRESOURCE_HPP

#include "SOGE/Sound/SoundCompilePreproc.hpp"
#include "SOGE/Content/ResourceBase.hpp"
#include <cppfs/FilePath.h>


namespace soge
{
    class SoundResource : public ResourceBase
    {
        SOGE_MAKE_SOUNDSYSTEM_CLASS_FRIEND(SoundCore)

    private:
        bool m_isPlaying;
        bool m_isPaused;
        bool m_isStopped;
        bool m_isLooping;
        bool m_is3d;

    protected:
        void SetActualLengthMs(unsigned int aLengthMs);

        void SetPlaying(bool aPlay) noexcept;
        void SetStopped(bool aStop) noexcept;
        void SetPaused(bool aPause) noexcept;

    public:
        SoundResource(const eastl::string_view& aName, const cppfs::FilePath& aFullPath);
        ~SoundResource() = default;

        bool Reload()  override;
        void Unload()  override;
        void Destroy() override;

        void Enable3D(bool aEnable3D) noexcept;
        void EnableLooping(bool aEnableLoop) noexcept;

        bool IsPlaying() const noexcept;
        bool IsPaused() const noexcept;
        bool IsStopped() const noexcept;
        bool IsLooping() const noexcept;
        bool Is3D() const noexcept;

    };
}

#endif // !SOGE_SOUND_SOUNDRESOURCE_HPP
