#ifndef SOGE_CORE_EVENT_EVENT_HPP
#define SOGE_CORE_EVENT_EVENT_HPP

#include "SOGE/Core/Event/EventType.hpp"


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
    };

    constexpr bool Event::IsHandled() const
    {
        return false;
    }
}

#endif // SOGE_CORE_EVENT_EVENT_HPP
