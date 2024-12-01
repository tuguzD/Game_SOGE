#ifndef SOGE_EVENT_COREEVENTS_HPP
#define SOGE_EVENT_COREEVENTS_HPP

#include "SOGE/Core/Event/Event.hpp"
#include "SOGE/Event/EventHelpers.hpp"


namespace soge
{
    namespace EventTypes
    {
        namespace CoreEvents
        {
            constexpr EventType g_updateEvent{StringId("UpdateEvent"), EventCategory::Core};
        }
    }

    class UpdateEvent : public StaticEvent<UpdateEvent>
    {
    private:
        float m_deltaTime;

    public:
        [[nodiscard]]
        static constexpr EventType GetStaticEventType() noexcept;

        explicit constexpr UpdateEvent(float aDeltaTime) noexcept;

        [[nodiscard]]
        constexpr float GetDeltaTime() const noexcept;
    };

    ////////////////////////////
    // Assertions
    ///////////////////////////

    SG_DERIVED_FROM_STATIC_EVENT_ASSERT(UpdateEvent);

    ////////////////////////////
    // Event declarations
    ///////////////////////////

    constexpr UpdateEvent::UpdateEvent(const float aDeltaTime) noexcept : m_deltaTime(aDeltaTime)
    {
    }

    constexpr EventType UpdateEvent::GetStaticEventType() noexcept
    {
        return EventTypes::CoreEvents::g_updateEvent;
    }

    constexpr float UpdateEvent::GetDeltaTime() const noexcept
    {
        return m_deltaTime;
    }
}

#endif // !SOGE_EVENT_COREEVENTS_HPP
