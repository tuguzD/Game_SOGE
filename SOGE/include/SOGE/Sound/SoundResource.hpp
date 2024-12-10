#ifndef SOGE_SOUND_SOUNDRESOURCE_HPP
#define SOGE_SOUND_SOUNDRESOURCE_HPP

#include "SOGE/Content/ResourceBase.hpp"
#include "SOGE/Containers/FriendFuncPtr.hpp"

#include <cppfs/FilePath.h>


namespace soge
{
    class SoundResource : public ResourceBase
    {
    private:
        unsigned int m_actualLengthMs;
        unsigned int m_userLengthMs;

        float m_reverbAmount;
        float m_volume;
        bool m_loop;
        bool m_is3d;

    protected:
        void SetActualLengthMs(unsigned int aLengthMs);

    public:
        SoundResource(const eastl::string& aName, const cppfs::FilePath& aFullPath);
        ~SoundResource() = default;

        bool Reload()  override;
        void Unload()  override;
        void Destroy() override;

        void SetLoop(bool aLoop);
        void Set3D(bool aNeed3D);
        void SetLength(unsigned int aLength);
        void SetVolume(float aVolume);

        bool IsLoop() const;
        bool Is3D() const;
        float GetVolume() const;
        float GetReverbAmount() const;
        unsigned int GetActualLengthMs() const;
        unsigned int GetUserLengthMs() const;

    public:
        static FriendFuncPtr<ResourceBase, void, bool> FriendlySetLoadState();
        static FriendFuncPtr<SoundResource, void, unsigned int> FriendlySetInitialLength();

    };
}

#endif // !SOGE_SOUND_SOUNDRESOURCE_HPP
