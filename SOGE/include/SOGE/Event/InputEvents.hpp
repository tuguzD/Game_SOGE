#ifndef SOGE_EVENT_INPUTEVENTS_HPP
#define SOGE_EVENT_INPUTEVENTS_HPP

#include "SOGE/Core/Event/Event.hpp"
#include "SOGE/Input/InputTypes.hpp"

namespace soge
{
    namespace EventTypes
    {
        namespace InputEvents
        {
            constexpr EventType g_keyPressedEvent{StringId("KeyPressedEvent"), EventCategory::Input};
            constexpr EventType g_keyReleasedEvent{StringId("KeyReleasedEvent"), EventCategory::Input};
        }
    }

    class KeyboardEventBase : public StaticEvent<KeyboardEventBase>
    {
    private:
        Key m_keyObj;

    public:
        explicit KeyboardEventBase(Key aKeyObj) noexcept;
        virtual ~KeyboardEventBase() = default;
        virtual Key GetKey() const;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

    };

    class KeyPressedEvent : public KeyboardEventBase
    {
    private:
        int m_repeatCount;

    public:
        explicit KeyPressedEvent(Key aPressedKey, int aRepeatCount) noexcept;
        ~KeyPressedEvent() = default;
        int GetRepeatCount() const;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        // this is needed because static event class overrides this method only from the first derived class, not the last one
        // how to reimplement this? for now, idk
        constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }
    };

    class KeyReleasedEvent : public KeyboardEventBase
    {
    public:
        explicit KeyReleasedEvent(Key aReleasedKey) noexcept;
        ~KeyReleasedEvent() = default;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        // this is needed because static event class overrides this method only from the first derived class, not the last one
        // how to reimplement this? for now, idk
        constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }
    };

    static_assert(DerivedFromStaticEvent<KeyboardEventBase>, "KeyboardEventBase should be static event!");

    ////////////////////////////
    // Event declarations
    ///////////////////////////

    constexpr EventType KeyboardEventBase::GetStaticEventType() noexcept
    {
        return EventTypes::Dummy::g_dummyEvent;
    }

    constexpr EventType KeyPressedEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_keyPressedEvent;
    }

    constexpr EventType KeyReleasedEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_keyReleasedEvent;
    }

}

#endif // SOGE_EVENT_INPUTEVENTS_HPP
