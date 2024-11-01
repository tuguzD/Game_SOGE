#ifndef SOGE_CORE_EVENTMANAGER_HPP
#define SOGE_CORE_EVENTMANAGER_HPP

#include "SOGE/Core/Events.hpp"

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

        static constexpr std::size_t s_anyEventMaxSize = eventpp::maxSizeOf<Event, UpdateEvent>();
        using AnyEvent = eventpp::AnyData<s_anyEventMaxSize>;

        using EventQueue = eventpp::EventQueue<EventType, void(AnyEvent&), Policies>;
        EventQueue m_eventQueue;

    public:
        using Function = EventQueue::Callback;
        using FunctionHandle = EventQueue::Handle;

        explicit EventManager() noexcept = default;
        ~EventManager() = default;

        explicit EventManager(const EventManager&) = default;
        EventManager& operator=(const EventManager&) = default;

        explicit EventManager(EventManager&&) noexcept = default;
        EventManager& operator=(EventManager&&) noexcept = default;

        // TODO push back, push front, insert, remove, etc.

        [[nodiscard]]
        bool IsEmpty() const;

        void Clear();
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

#endif // SOGE_CORE_EVENTMANAGER_HPP
