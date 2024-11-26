#ifndef SOGE_CORE_EVENT_EVENT_HPP
#define SOGE_CORE_EVENT_EVENT_HPP

#include "SOGE/Core/Event/EventType.hpp"


namespace soge
{
    class Event;

    template <typename T>
    concept DerivedFromEvent = std::is_base_of_v<Event, T>;

    class Event
    {
    public:
        constexpr Event() = default;
        virtual constexpr ~Event() = default;

        [[nodiscard]]
        virtual constexpr EventType GetEventType() const = 0;
        [[nodiscard]]
        virtual constexpr bool IsHandled() const;
    };

    constexpr bool Event::IsHandled() const
    {
        return false;
    }

    template <typename Derived>
    class StaticEvent;

    template <typename T>
    concept DerivedFromStaticEvent = std::is_base_of_v<StaticEvent<T>, T> && requires {
        // clang-format off
        { T::GetStaticEventType() } -> std::same_as<EventType>;
        // clang-format on
    };

    template <typename Derived>
    class StaticEvent : public Event
    {
    protected:
        explicit constexpr StaticEvent() noexcept = default;

    public:
        [[nodiscard]]
        constexpr EventType GetEventType() const override;
    };

    template <typename Derived>
    constexpr EventType StaticEvent<Derived>::GetEventType() const
    {
        return Derived::GetStaticEventType();
    }

    namespace EventTypes
    {
        namespace Dummy
        {
            constexpr EventType g_dummyEvent{StringId("DummyEvent"), EventCategory::Default};
        }
    }

    typedef std::function<void(Event&)> DefaultEventCallback;
}

#endif // SOGE_CORE_EVENT_EVENT_HPP
