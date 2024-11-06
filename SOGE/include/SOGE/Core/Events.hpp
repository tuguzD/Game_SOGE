#ifndef SOGE_CORE_EVENTS_HPP
#define SOGE_CORE_EVENTS_HPP

#include "SOGE/Core/Event/Event.hpp"


namespace soge
{
    class UpdateEvent : public StaticEvent<UpdateEvent>
    {
    private:
        float m_deltaTime;

    public:
        static constexpr EventType s_eventType{StringId("Update"), EventCategory::Core};

        [[nodiscard]]
        static constexpr EventType GetStaticEventType() noexcept;

        explicit constexpr UpdateEvent(float aDeltaTime) noexcept;

        [[nodiscard]]
        constexpr float GetDeltaTime() const noexcept;
    };

    static_assert(DerivedFromStaticEvent<UpdateEvent>, "Update core event should be static event");

    constexpr EventType UpdateEvent::GetStaticEventType() noexcept
    {
        return s_eventType;
    }

    constexpr UpdateEvent::UpdateEvent(const float aDeltaTime) noexcept : m_deltaTime(aDeltaTime)
    {
    }

    constexpr float UpdateEvent::GetDeltaTime() const noexcept
    {
        return m_deltaTime;
    }
}

#endif // SOGE_CORE_EVENTS_HPP
