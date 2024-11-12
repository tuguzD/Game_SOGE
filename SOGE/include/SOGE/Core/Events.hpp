#ifndef SOGE_CORE_EVENTS_HPP
#define SOGE_CORE_EVENTS_HPP

#include "SOGE/Core/Event/Event.hpp"


namespace soge
{
    namespace EventTypes
    {
        namespace CoreEvents
        {
            constexpr EventType g_updateEvent{StringId("UpdateEvent"), EventCategory::Core};
        }

        namespace InputEvents
        {
            constexpr EventType g_keyPressedEvent{StringId("KeyPressedEvent"), EventCategory::Input};
            constexpr EventType g_keyReleasedEvent{StringId("KeyReleasedEvent"), EventCategory::Input};
            constexpr EventType g_keyTypingEvent{StringId("KeyTypingEvent"), EventCategory::Input};

            constexpr EventType g_mouseMotionEvent{StringId("MouseMotionEvent"), EventCategory::Input};
            constexpr EventType g_mouseButtonPressedEvent{StringId("MouseButtonPressedEvent"), EventCategory::Input};
            constexpr EventType g_mouseButtonReleasedEvent{StringId("MouseButtonReleasedEvent"), EventCategory::Input};
            constexpr EventType g_mouseWheelEvent{StringId("MouseWheelEvent"), EventCategory::Input};

            // TODO: Add DeviceAdded and DeviceRemoved event for each device in the future
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

    class KeyboardEventBase : public StaticEvent<KeyboardEventBase>
    {
    private:

    public:


    public:
        static constexpr EventType GetStaticEventType() noexcept;
    };

    static_assert(DerivedFromStaticEvent<UpdateEvent>, "Update core event should be static event!");
    static_assert(DerivedFromStaticEvent<KeyboardEventBase>, "Keyboard event should be static event!");

    ////////////////////////////
    // Update Event Templates
    ///////////////////////////

    constexpr EventType UpdateEvent::GetStaticEventType() noexcept
    {
        return EventTypes::CoreEvents::g_updateEvent;
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
