#ifndef SOGE_SOUND_SOUNDMODULE_HPP
#define SOGE_SOUND_SOUNDMODULE_HPP

#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Sound/Public/ChannelMixer.hpp"
#include "SOGE/Sound/SoundCore.hpp"


namespace soge
{
    class SoundModule : public Module
    {
    private:
        SoundCore* m_soundCore;

    public:
        explicit SoundModule();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;

        ChannelMixer* GetChannelMixer() const;
        SoundResource* CreateSoundResource(const eastl::string_view& aName, const cppfs::FilePath& aFilePath,
                                           bool aIs3D);

        void LoadSoundResource(SoundResource* aSoundResource) const;
        void PlaySoundResource(SoundResource* aSoundResource) const;
        void PauseSoundResource(SoundResource* aSoundResource) const;
        void StopSoundResource(SoundResource* aSoundResource) const;

        void Update() const;
        void Update3DListener(const glm::vec3& aPosition, const glm::vec3& aForwardVec,
                              const glm::vec3& aUpwardVec) const;
    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, SoundModule)

#endif // SOGE_SOUND_SOUNDMODULE_HPP
