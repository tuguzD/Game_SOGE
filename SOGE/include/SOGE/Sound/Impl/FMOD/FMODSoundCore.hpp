#ifndef SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP

#include "SOGE/Sound/SoundCore.hpp"
#include "SOGE/Utils/UUID.hpp"

#include <FMOD/fmod.hpp>
#include <FMOD/fmod_studio.hpp>


namespace soge
{
    class FMODSoundCore : public SoundCore
    {
        struct FMODConfig
        {
            const float DISTANCE_FACTOR             = 1.0f;
            const unsigned int MAX_AUDIO_CHANNELS   = 255;
            const int AUDIO_SAMPLE_RATE             = 44100;
        };

    private:
        // FMOD system variables
        FMOD::Studio::System* m_fmodStudioSystem;
        FMOD::System* m_fmodSystem;

        // FMOD feature variables
        FMOD::ChannelGroup* m_masterGroup;
        FMOD::Reverb3D* m_fmodReverb;

        // Sound engine variables
        FMODConfig m_config;
        bool m_muted;

        // Reverb variables
        float m_reverbMinDistance = 10.0f;
        float m_reverbMaxDistance = 50.0f;
        FMOD_VECTOR m_reverbPosition;

        // Listener variables
        FMOD_VECTOR m_listenerHeadPosition;
        FMOD_VECTOR m_listenerForwardVector;
        FMOD_VECTOR m_listenerUpVector;

        // FMOD containers
        eastl::map<UUIDv4::UUID, FMOD::Sound*> m_sounds;
        eastl::map<UUIDv4::UUID, FMOD::Channel*> m_loopsPlaying;
        eastl::map<UUIDv4::UUID, FMOD::Studio::Bank*> m_soundBanks;
        eastl::map<UUIDv4::UUID, FMOD::Studio::EventDescription*> m_eventDescriptions;
        eastl::map<UUIDv4::UUID, FMOD::Studio::EventInstance*> m_eventInstances;

    private:
        void InitReverb();

    public:
        explicit FMODSoundCore(EventModule& aEventModule);
        ~FMODSoundCore();

        void BeginUpdateSound() override;
        void EndUpdateSound() override;
        void SetPauseUpdate(float aPauseRequested) override;

        void Set3DListenerPosition(float aPosX,     float aPosY,     float aPosZ,
                                   float aForwardX, float aForwardY, float aForwardZ,
                                   float aUpX,      float aUpY,      float aUpZ) override;

        void LoadSoundResource(SoundResource& aSoundResource) override;
        void UnloadSoundResource(SoundResource& aSoundResource) override;
        void ReloadSoundResource(SoundResource& aSoundResource) override;

        void Dummy(){return;}

        void PlaySoundResource(SoundResource& aSoundResource) override;
        void StopSoundResource(SoundResource& aSoundResource) override;

        bool IsSoundPlaying(SoundResource& aSoundResource) override;

        void SetVolume(float aVolume);
        float GetVolume() const override;

    };

    using ImplSoundCore = FMODSoundCore;
}

SOGE_DI_REGISTER_NS(soge, FMODSoundCore, df::Single<FMODSoundCore, EventModule>, tag::Overrides<SoundCore>)

#endif // !SOGE_SOUND_IMPL_FMOD_FMODSOUNDCORE_HPP
