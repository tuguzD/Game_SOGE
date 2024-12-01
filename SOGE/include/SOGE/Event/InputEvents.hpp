#ifndef SOGE_EVENT_INPUTEVENTS_HPP
#define SOGE_EVENT_INPUTEVENTS_HPP

#include "SOGE/Core/Event/Event.hpp"
#include "SOGE/Core/EngineTypes.hpp"
#include "SOGE/Input/InputTypes.hpp"
#include "SOGE/Event/EventHelpers.hpp"


namespace soge
{
    namespace EventTypes
    {
        namespace InputEvents
        {
            constexpr EventType g_keyPressedEvent{StringId("KeyPressedEvent"), EventCategory::Input};
            constexpr EventType g_keyReleasedEvent{StringId("KeyReleasedEvent"), EventCategory::Input};

            constexpr EventType g_mouseButtonPressedEvent{StringId("MouseButtonPressedEvent"), EventCategory::Input};
            constexpr EventType g_mouseButtonReleasedEvent{StringId("MouseButtonReleasedEvent"), EventCategory::Input};
            constexpr EventType g_mouseMovedEvent{StringId("MouseMovedEvent"), EventCategory::Input};
            constexpr EventType g_mouseWheelEvent{StringId("MouseWheelEvent"), EventCategory::Input};
        }
    }

    class KeyEventBase : public StaticEvent<KeyEventBase>
    {
    private:
        Key m_keyObj;

    public:
        explicit KeyEventBase(const Key aKeyObj) noexcept;
        virtual ~KeyEventBase() = default;
        virtual Key GetKey() const;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

    };

    //////////////////////
    // Keyboard Event
    /////////////////////

    class KeyPressedEvent : public KeyEventBase
    {
    private:
        int m_repeatCount;

    public:
        explicit KeyPressedEvent(const Key aPressedKey, int aRepeatCount) noexcept;
        ~KeyPressedEvent() = default;
        int GetRepeatCount() const;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        // this is needed because static event class overrides this method only from the first derived class, not the last one
        // how to reimplement this? for now, idk
        inline constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }
    };

    class KeyReleasedEvent : public KeyEventBase
    {
    public:
        explicit KeyReleasedEvent(const Key aReleasedKey) noexcept;
        ~KeyReleasedEvent() = default;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        // this is needed because static event class overrides this method only from the first derived class, not the last one
        // how to reimplement this? for now, idk
        inline constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }
    };

    //////////////////////
    // Mouse Event
    /////////////////////

    // Pretty much similar to keyboard key pressed event.
    // But we need set exactly g_mouseButtonKeyPressedEvent.
    // So here'll be some code duplication. Probably can be
    // Moved to some kind of multifunction for all button events.
    //
    // TODO: Figure out this garbage in the future

    class MouseButtonPressedEvent : public KeyEventBase
    {
    private:
        int m_repeatCount;

    public:
        explicit MouseButtonPressedEvent(const Key aPressedButton, int aRepeatCount) noexcept;
        ~MouseButtonPressedEvent() = default;
        int GetRepeatCount() const;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        inline constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }

    };

    class MouseButtonReleasedEvent : public KeyEventBase
    {
    public:
        explicit MouseButtonReleasedEvent(const Key aReleasedButton) noexcept;
        ~MouseButtonReleasedEvent() = default;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        inline constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }

    };

    class MouseMovedEvent : public StaticEvent<MouseMovedEvent>
    {
        using FloatPair = eastl::pair<float, float>;

    private:
        FloatPair m_coords;


    public:
        explicit MouseMovedEvent(const FloatPair aCoords) noexcept;
        explicit MouseMovedEvent(float aX, float aY) noexcept;
        ~MouseMovedEvent() = default;
        FloatPair GetCoords() const;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        inline constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }

    };

    class MouseWheelEvent : public StaticEvent<MouseWheelEvent>
    {
    private:
        float m_xOffset;
        float m_yOffset;

    public:
        explicit MouseWheelEvent(float aXOffset, float aYOffset) noexcept;
        ~MouseWheelEvent() = default;

        float GetXOffset() const;
        float GetYOffset() const;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        inline constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }

    };

    //////////////////////
    // Assertions
    /////////////////////

    SG_DERIVED_FROM_STATIC_EVENT_ASSERT(KeyEventBase);
    SG_DERIVED_FROM_STATIC_EVENT_ASSERT(KeyPressedEvent);
    SG_DERIVED_FROM_STATIC_EVENT_ASSERT(KeyReleasedEvent);
    SG_DERIVED_FROM_STATIC_EVENT_ASSERT(MouseButtonPressedEvent);
    SG_DERIVED_FROM_STATIC_EVENT_ASSERT(MouseButtonReleasedEvent);
    SG_DERIVED_FROM_STATIC_EVENT_ASSERT(MouseMovedEvent);
    SG_DERIVED_FROM_STATIC_EVENT_ASSERT(MouseWheelEvent);

    ////////////////////////////
    // Event declarations
    ///////////////////////////

    constexpr EventType KeyEventBase::GetStaticEventType() noexcept
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

    constexpr EventType MouseButtonPressedEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_mouseButtonPressedEvent;
    }

    constexpr EventType MouseButtonReleasedEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_mouseButtonReleasedEvent;
    }

    constexpr EventType MouseMovedEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_mouseMovedEvent;
    }

    constexpr EventType MouseWheelEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_mouseWheelEvent;
    }

}

#endif // SOGE_EVENT_INPUTEVENTS_HPP
