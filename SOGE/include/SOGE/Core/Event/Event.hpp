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
}

#endif // SOGE_CORE_EVENT_EVENT_HPP
