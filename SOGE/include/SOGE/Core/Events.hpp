#ifndef SOGE_CORE_EVENTS_HPP
#define SOGE_CORE_EVENTS_HPP

#include "SOGE/Core/Event/Event.hpp"


namespace soge
{
    class UpdateEvent : public Event
    {
    private:
        float m_deltaTime;

    public:
        static constexpr EventType s_eventType{StringId("Update"), EventCategory::Default};

        explicit constexpr UpdateEvent(float aDeltaTime) noexcept;

        [[nodiscard]]
        constexpr float GetDeltaTime() const noexcept;

        [[nodiscard]]
        constexpr EventType GetEventType() const override;
    };

    constexpr UpdateEvent::UpdateEvent(const float aDeltaTime) noexcept : m_deltaTime(aDeltaTime)
    {
    }

    constexpr float UpdateEvent::GetDeltaTime() const noexcept
    {
        return m_deltaTime;
    }

    constexpr EventType UpdateEvent::GetEventType() const
    {
        return s_eventType;
    }
}

#endif // SOGE_CORE_EVENTS_HPP
