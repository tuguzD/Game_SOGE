#ifndef SOGE_SOUND_SOUNDCORE_HPP
#define SOGE_SOUND_SOUNDCORE_HPP


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
        float m_volume;

    public:
        constexpr explicit SoundCore(EventModule* aEventModule) noexcept;
        constexpr virtual ~SoundCore() noexcept = default;

        constexpr explicit SoundCore(const SoundCore&) noexcept = delete;
        constexpr SoundCore& operator=(const SoundCore&) noexcept = delete;

        constexpr explicit SoundCore(SoundCore&&) noexcept = default;
        constexpr SoundCore& operator=(SoundCore&&) noexcept = default;

        virtual void BeginUpdateSound() = 0;
        virtual void EndUpdateSound()   = 0;
        virtual void SetPauseUpdate()   = 0;

        virtual void SetVolume(float aVolume) = 0;
        float GetVolume() const;

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
