#ifndef SOGE_CORE_EVENT_HPP
#define SOGE_CORE_EVENT_HPP

#include "SOGE/Core/EventType.hpp"


namespace soge
{
    class Event
    {
    public:
        virtual constexpr ~Event() = default;

        [[nodiscard]]
        virtual constexpr EventType GetEventType() const = 0;

        [[nodiscard]]
        virtual constexpr bool IsHandled() const;
        virtual constexpr void Handle();
    };

    constexpr bool Event::IsHandled() const
    {
        return false;
    }

    constexpr void Event::Handle()
    {
    }
}

#endif // SOGE_CORE_EVENT_HPP
