#ifndef SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP

#include "SOGE/Sound/SoundCore.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSound.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODConfig.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODSoundChannel.hpp"

#include <FMOD/fmod.hpp>
#include <FMOD/fmod_studio.hpp>


namespace soge
{
    class FMODSystem final
    {
        friend class FMODSoundCore;
        friend class FMODChannelMixer;

    private:
        static SharedPtr<FMODSystem> s_instance;
        FMOD::Studio::System* m_fmodStudioSystem;
        FMOD::System* m_fmodSystem;

    protected:
        FMODSystem();

        FMODSoundChannel* m_defaultSoundChannel;
        eastl::map<UUIDv4::UUID, FMODSound*> m_loadedSounds;

    public:
        FMODSystem(FMODSystem&)     = delete;
        FMODSystem(FMODSystem&&)    = delete;

       auto operator= (FMODSystem&)     = delete;
       auto operator= (FMODSystem&&)    = delete;
       ~FMODSystem();

       void Initialize();
       void Destroy();

       FMOD::Studio::System* GetStudioSystem() const;
       FMOD::System* GetSystem() const;

    public:
       static FMODSystem* GetInstance();

    };

    class Listener;

    class FMODSoundCore final : public SoundCore
    {
    private:

        FMODConfig m_config;
        FMODSystem* m_fmodSystem;
        SharedPtr<Listener> m_listener;

    public:
        explicit FMODSoundCore(EventModule& aEventModule);
        ~FMODSoundCore();

        void Update() override;
        void Update3DListener(const glm::vec3& aPos,
                              const glm::vec3& aForwardVec,
                              const glm::vec3& aUpwardVec) override;

        ChannelMixer* GetChannelMixer() override;
        SoundResource* CreateSoundResource(const eastl::string_view& aName, const cppfs::FilePath& aFilePath,
                                           bool aIs3D) override;

        void LoadSoundResource(SoundResource* aSoundResource) override;
        void PlaySoundResource(SoundResource* aSoundResource) override;
        void PauseSoundResource(SoundResource* aSoundResource) override;
        void UnpauseSoundResource(SoundResource* aSoundResource) override;
        void StopSoundResource(SoundResource* aSoundResource) override;
        bool IsSoundResourcePlaying(SoundResource* aSoundResource) override;
    };

    using ImplSoundCore = soge::FMODSoundCore;
}

SOGE_DI_REGISTER_NS(soge, FMODSoundCore, df::Single<FMODSoundCore, EventModule>, tag::Overrides<SoundCore>)

#endif // !SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP
