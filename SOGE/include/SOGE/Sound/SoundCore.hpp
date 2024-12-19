#ifndef SOGE_SOUND_SOUNDCORE_HPP
#define SOGE_SOUND_SOUNDCORE_HPP

#include "SOGE/Sound/SoundResource.hpp"
#include "SOGE/Event/EventModule.hpp"


namespace soge
{
    class SoundCore
    {
    private:
        eastl::reference_wrapper<EventModule> m_eventModule;

    protected:
        [[nodiscard]]
        EventModule& GetEventModule() const noexcept;

        bool m_isPauseUpdateRequested;
        bool m_isEndUpdateRequested;

    public:
        explicit SoundCore(EventModule& aEventModule) noexcept;
        constexpr virtual ~SoundCore() noexcept = default;

        constexpr explicit SoundCore(const SoundCore&) noexcept = delete;
        constexpr SoundCore& operator=(const SoundCore&) noexcept = delete;

        constexpr explicit SoundCore(SoundCore&&) noexcept = default;
        constexpr SoundCore& operator=(SoundCore&&) noexcept = default;

        virtual void Update() = 0;

        virtual void LoadSoundResource(SoundResource& aSoundResource) = 0;
        virtual void PlaySoundResource(SoundResource& aSoundResource) = 0;
        virtual bool IsSoundResourcePlaying(SoundResource& aSoundResource) = 0;
        //void Stop(const SoundResource& aSoundResource);

    };

    inline SoundCore::SoundCore(EventModule& aEventModule) noexcept : m_eventModule(aEventModule)
    {
    }

    inline EventModule& SoundCore::GetEventModule() const noexcept
    {
        return m_eventModule.get();
    }
}

SOGE_DI_REGISTER_NS(soge, SoundCore, df::Abstract<SoundCore>)

#endif // !SOGE_SOUND_SOUNDCORE_HPP
