#ifndef SOGE_SOUND_SOUNDCORE_HPP
#define SOGE_SOUND_SOUNDCORE_HPP

#include "SOGE/Sound/SoundResource.hpp"


namespace soge
{
    class EventModule;

    class SoundCore
    {
    private:
        EventModule* m_eventModule;

    protected:
        [[nodiscard]]
        constexpr EventModule* GetEventModule() const noexcept;

        bool m_isPauseUpdateRequested;
        bool m_isEndUpdateRequested;

    public:
        constexpr explicit SoundCore(EventModule* aEventModule) noexcept;
        constexpr virtual ~SoundCore() noexcept = default;

        constexpr explicit SoundCore(const SoundCore&) noexcept = delete;
        constexpr SoundCore& operator=(const SoundCore&) noexcept = delete;

        constexpr explicit SoundCore(SoundCore&&) noexcept = default;
        constexpr SoundCore& operator=(SoundCore&&) noexcept = default;

        virtual void BeginUpdateSound()                         = 0;
        virtual void EndUpdateSound()                           = 0;
        virtual void SetPauseUpdate(float aPauseRequested)      = 0;

        virtual void Set3DListenerPosition(float aPosX,     float aPosY,     float aPosZ,
                                           float aForwardX, float aForwardY, float aForwardZ,
                                           float aUpX,      float aUpY,      float aUpZ) = 0;

        virtual void LoadSoundResource(SoundResource& aSoundResource)     = 0;
        virtual void UnloadSoundResource(SoundResource& aSoundResource)   = 0;
        virtual void ReloadSoundResource(SoundResource& aSoundResource)   = 0;

        virtual void PlaySoundResource(SoundResource& aSoundResource) = 0;
        virtual void StopSoundResource(SoundResource& aSoundResource) = 0;

        virtual bool IsSoundPlaying(SoundResource& aSoundResource) = 0;

        virtual void SetVolume(float aVolume) = 0;
        virtual float GetVolume() const = 0;

    };

    constexpr SoundCore::SoundCore(EventModule* aEventModule) noexcept : m_eventModule(aEventModule)
    {
    }

    constexpr EventModule* SoundCore::GetEventModule() const noexcept
    {
        return m_eventModule;
    }
}

#endif // !SOGE_SOUND_SOUNDCORE_HPP
