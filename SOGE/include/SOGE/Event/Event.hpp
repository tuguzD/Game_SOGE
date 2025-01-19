#ifndef SOGE_EVENT_EVENT_HPP
#define SOGE_EVENT_EVENT_HPP

#include "SOGE/Event/EventType.hpp"


namespace soge
{
    class Event;

    template <typename T>
    concept DerivedFromEvent = std::derived_from<T, Event>;

    class Event
    {
    public:
        constexpr explicit Event() noexcept = default;

        constexpr explicit Event(const Event&) noexcept = default;
        Event& operator=(const Event&) = default;

        constexpr explicit Event(Event&&) noexcept = default;
        Event& operator=(Event&&) = default;

        virtual constexpr ~Event() noexcept = default;

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

    namespace detail
    {
        template <typename T>
        constexpr void StaticEventConceptHelper(StaticEvent<T>&) noexcept
        {
        }
    }

    template <typename T>
    concept DerivedFromStaticEvent = DerivedFromEvent<T> && requires(T&& aT) {
        { detail::StaticEventConceptHelper(aT) };
        { T::GetStaticEventType() } -> std::same_as<EventType>;
    };

    template <typename Derived>
    class StaticEvent : public Event
    {
    private:
        friend Derived;

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

    // NOLINTNEXTLINE(readability-identifier-naming)
    namespace EventTypes
    {
        // NOLINTNEXTLINE(readability-identifier-naming)
        namespace Dummy
        {
            constexpr EventType g_dummyEvent{StringId("DummyEvent"), EventCategory::Default};
        }
    }
}

#endif // SOGE_EVENT_EVENT_HPP
