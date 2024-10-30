#ifndef SOGE_CORE_EVENT_EVENTPOLICIES_HPP
#define SOGE_CORE_EVENT_EVENTPOLICIES_HPP

#include "SOGE/Core/Event/Event.hpp"

#include <eventpp/eventpolicies.h>


namespace soge
{
    class EventPolicies
    {
    private:
        explicit EventPolicies() = default;

    public:
        using Prototype = void(Event&);
        using ArgumentPassingMode = eventpp::ArgumentPassingIncludeEvent;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: required by `eventpp` library
        static constexpr EventType getEvent(const Event& aEvent);

        // NOLINTNEXTLINE(readability-identifier-naming) reason: required by `eventpp` library
        static constexpr bool canContinueInvoking(const Event& aEvent);
    };

    constexpr EventType EventPolicies::getEvent(const Event& aEvent)
    {
        return aEvent.GetEventType();
    }

    constexpr bool EventPolicies::canContinueInvoking(const Event& aEvent)
    {
        return !aEvent.IsHandled();
    }
}

#endif // SOGE_CORE_EVENT_EVENTPOLICIES_HPP
