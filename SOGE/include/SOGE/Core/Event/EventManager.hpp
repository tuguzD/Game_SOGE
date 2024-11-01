#ifndef SOGE_CORE_EVENT_EVENTMANAGER_HPP
#define SOGE_CORE_EVENT_EVENTMANAGER_HPP

#include "SOGE/Core/Event/Event.hpp"

#include <eventpp/eventqueue.h>
#include <eventpp/utilities/anydata.h>


namespace soge
{
    class EventManager
    {
    private:
        class Policies
        {
        private:
            explicit Policies() = default;

        public:
            using ArgumentPassingMode = eventpp::ArgumentPassingIncludeEvent;

            // NOLINTNEXTLINE(readability-identifier-naming) reason: required by `eventpp` library
            static constexpr EventType getEvent(const Event& aEvent);

            // NOLINTNEXTLINE(readability-identifier-naming) reason: required by `eventpp` library
            static constexpr bool canContinueInvoking(const Event& aEvent);
        };

        static constexpr std::size_t s_maxStackSize = eventpp::maxSizeOf<Event>();
        using AnyEvent = eventpp::AnyData<s_maxStackSize>;

        eventpp::EventQueue<EventType, void(AnyEvent&), Policies> m_events;

    public:
    };

    constexpr EventType EventManager::Policies::getEvent(const Event& aEvent)
    {
        return aEvent.GetEventType();
    }

    constexpr bool EventManager::Policies::canContinueInvoking(const Event& aEvent)
    {
        return !aEvent.IsHandled();
    }
}

#endif // SOGE_CORE_EVENT_EVENTMANAGER_HPP
