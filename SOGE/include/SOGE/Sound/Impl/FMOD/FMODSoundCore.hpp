#ifndef SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP

#include "SOGE/Sound/SoundCore.hpp"
#include "SOGE/Utils/UUID.hpp"

#include <FMOD/fmod.hpp>
#include <FMODStudio/fmod_studio.hpp>


namespace soge
{
    class FMODSoundCore : public SoundCore
    {
    private:
        FMOD::Studio::System* m_fmodStudioSystem;
        FMOD::System* m_fmodSystem;
        FMOD::ChannelGroup* m_masterGroup;
        int m_maxAudioChannels = 1024;

        float m_distanceFactor;
        FMOD_VECTOR m_listenerHeadPosition;
        FMOD_VECTOR m_listenerForwardVector;
        FMOD_VECTOR m_listenerUpVector;

        eastl::map<UUID, FMOD::Sound*> m_sounds;
        eastl::map<UUID, FMOD::Channel*> m_loopsPlaying;


    public:
        FMODSoundCore(EventModule* aEventModule);
        ~FMODSoundCore();

        void BeginUpdateSound() override;
        void EndUpdateSound() override;
        void SetPauseUpdate(float aPauseRequested) override;

        void Set3DListenerPosition(float aPosX,     float aPosY,     float aPosZ,
                                   float aForwardX, float aForwardY, float aForwardZ,
                                   float aUpX,      float aUpY,      float aUpZ) override;

        void LoadSoundResource(const SoundResource& aSoundResource) override;
        void UnloadSoundResource(const SoundResource& aSoundResource) override;
        void ReloadSoundResource(const SoundResource& aSoundResource) override;

        void PlaySoundResource(const SoundResource& aSoundResource) override;
        void StopSoundResource(const SoundResource& aSoundResource) override;

        bool IsSoundPlaying(const SoundResource& aSoundResource) override;

        void SetVolume(float aVolume);
        float GetVolume() const override;

    };

    using ImplSoundCore = FMODSoundCore;
}

#endif // !SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP
