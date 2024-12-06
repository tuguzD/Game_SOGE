#ifndef SOGE_CORE_EVENTMANAGER_HPP
#define SOGE_CORE_EVENTMANAGER_HPP

#include "SOGE/Core/Event/Event.hpp"
#include "SOGE/Event/CoreEvents.hpp"

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

        class AnyEvent final : public Event
        {
        private:
            static constexpr std::size_t s_anyEventMaxSize = eventpp::maxSizeOf<Event, UpdateEvent>();
            eventpp::AnyData<s_anyEventMaxSize> m_data;

        public:
            template <DerivedFromEvent E>
            explicit AnyEvent(E&& aEvent);

            explicit AnyEvent(const AnyEvent&) = delete;
            AnyEvent& operator=(const AnyEvent&) = delete;

            explicit AnyEvent(AnyEvent&&) = default;
            AnyEvent& operator=(AnyEvent&&) noexcept = delete;

            ~AnyEvent() override = default;

            [[nodiscard]]
            EventType GetEventType() const override;

            [[nodiscard]]
            bool IsHandled() const override;

            template <DerivedFromEvent E = Event>
            operator E&() const;
        };

        using EventQueue = eventpp::EventQueue<EventType, void(AnyEvent&), Policies>;
        EventQueue m_eventQueue;

    public:
        using Function = EventQueue::Callback;
        class FunctionHandle;

        explicit EventManager() noexcept = default;
        ~EventManager() = default;

        explicit EventManager(const EventManager&) = default;
        EventManager& operator=(const EventManager&) = default;

        explicit EventManager(EventManager&&) noexcept = default;
        EventManager& operator=(EventManager&&) noexcept = default;

        template <DerivedFromStaticEvent E, typename F>
        requires std::is_invocable_v<F, E&>
        FunctionHandle PushBack(F&& aFunction);

        template <typename F>
        requires std::is_invocable_v<F, Event&>
        FunctionHandle PushBack(const EventType& aEventType, F&& aFunction);

        template <DerivedFromStaticEvent E, typename F>
        requires std::is_invocable_v<F, E&>
        FunctionHandle PushFront(F&& aFunction);

        template <typename F>
        requires std::is_invocable_v<F, Event&>
        FunctionHandle PushFront(const EventType& aEventType, F&& aFunction);

        template <DerivedFromStaticEvent E, typename F>
        requires std::is_invocable_v<F, E&>
        FunctionHandle Insert(F&& aFunction, const FunctionHandle& aBefore);

        template <typename F>
        requires std::is_invocable_v<F, Event&>
        FunctionHandle Insert(const EventType& aEventType, F&& aFunction, const FunctionHandle& aBefore);

        bool Remove(const FunctionHandle& aHandle);

        [[nodiscard]]
        bool Contains(const FunctionHandle& aHandle) const;

        [[nodiscard]]
        bool IsQueueEmpty() const;

        template <DerivedFromStaticEvent E>
        [[nodiscard]]
        bool IsEmpty() const;

        [[nodiscard]]
        bool IsEmpty(const EventType& aEventType) const;

        void ClearQueue();
        void Clear();

        template <DerivedFromEvent E, typename... Args>
        requires std::is_constructible_v<E, Args...>
        void Dispatch(Args&&... args);

        template <DerivedFromEvent E, typename... Args>
        requires std::is_constructible_v<E, Args...>
        void Enqueue(Args&&... args);

        template <DerivedFromStaticEvent E>
        void DispatchQueue();

        void DispatchQueue(const EventType& aEventType);
        void DispatchQueue();
    };

    class EventManager::FunctionHandle : public EventQueue::Handle
    {
    private:
        using Parent = EventQueue::Handle;
        friend EventManager;

        explicit FunctionHandle(Parent&& aHandle, const EventType& aEventType) noexcept;

        EventType m_eventType;

    public:
        [[nodiscard]]
        const EventType& GetEventType() const noexcept;
    };

    constexpr EventType EventManager::Policies::getEvent(const Event& aEvent)
    {
        return aEvent.GetEventType();
    }

    constexpr bool EventManager::Policies::canContinueInvoking(const Event& aEvent)
    {
        return !aEvent.IsHandled();
    }

    template <DerivedFromEvent E>
    EventManager::AnyEvent::AnyEvent(E&& aEvent) : m_data(std::forward<E>(aEvent))
    {
    }

    template <DerivedFromEvent E>
    EventManager::AnyEvent::operator E&() const
    {
        return static_cast<E&>(m_data);
    }

    template <DerivedFromStaticEvent E, typename F>
    requires std::is_invocable_v<F, E&>
    auto EventManager::PushBack(F&& aFunction) -> FunctionHandle
    {
        const EventType eventType = E::GetStaticEventType();

        FunctionHandle::Parent handle = m_eventQueue.appendListener(eventType, std::forward<F>(aFunction));
        return FunctionHandle{std::move(handle), eventType};
    }

    template <typename F>
    requires std::is_invocable_v<F, Event&>
    auto EventManager::PushBack(const EventType& aEventType, F&& aFunction) -> FunctionHandle
    {
        FunctionHandle::Parent handle = m_eventQueue.appendListener(aEventType, std::forward<F>(aFunction));
        return FunctionHandle{std::move(handle), aEventType};
    }

    template <DerivedFromStaticEvent E, typename F>
    requires std::is_invocable_v<F, E&>
    auto EventManager::PushFront(F&& aFunction) -> FunctionHandle
    {
        const EventType eventType = E::GetStaticEventType();

        FunctionHandle::Parent handle = m_eventQueue.prependListener(eventType, std::forward<F>(aFunction));
        return FunctionHandle{std::move(handle), eventType};
    }

    template <typename F>
    requires std::is_invocable_v<F, Event&>
    auto EventManager::PushFront(const EventType& aEventType, F&& aFunction) -> FunctionHandle
    {
        FunctionHandle::Parent handle = m_eventQueue.prependListener(aEventType, std::forward<F>(aFunction));
        return FunctionHandle{std::move(handle), aEventType};
    }

    template <DerivedFromStaticEvent E, typename F>
    requires std::is_invocable_v<F, E&>
    auto EventManager::Insert(F&& aFunction, const FunctionHandle& aBefore) -> FunctionHandle
    {
        const EventType eventType = E::GetStaticEventType();

        FunctionHandle::Parent handle =
            m_eventQueue.ownsHandle(eventType, aBefore)
                ? m_eventQueue.insertListener(eventType, std::forward<F>(aFunction), aBefore)
                : m_eventQueue.appendListener(eventType, std::forward<F>(aFunction));
        return FunctionHandle{std::move(handle), eventType};
    }

    template <typename F>
    requires std::is_invocable_v<F, Event&>
    auto EventManager::Insert(const EventType& aEventType, F&& aFunction, const FunctionHandle& aBefore)
        -> FunctionHandle
    {
        FunctionHandle::Parent handle =
            m_eventQueue.ownsHandle(aEventType, aBefore)
                ? m_eventQueue.insertListener(aEventType, std::forward<F>(aFunction), aBefore)
                : m_eventQueue.appendListener(aEventType, std::forward<F>(aFunction));
        return FunctionHandle{std::move(handle), aEventType};
    }

    template <DerivedFromStaticEvent E>
    bool EventManager::IsEmpty() const
    {
        const EventType eventType = E::GetStaticEventType();
        return IsEmpty(eventType);
    }

    template <DerivedFromEvent E, typename... Args>
    requires std::is_constructible_v<E, Args...>
    void EventManager::Dispatch(Args&&... args)
    {
        AnyEvent event{E{std::forward<Args>(args)...}};
        m_eventQueue.dispatch(event);
    }

    template <DerivedFromEvent E, typename... Args>
    requires std::is_constructible_v<E, Args...>
    void EventManager::Enqueue(Args&&... args)
    {
        AnyEvent event{E{std::forward<Args>(args)...}};
        m_eventQueue.enqueue(std::move(event));
    }

    template <DerivedFromStaticEvent E>
    void EventManager::DispatchQueue()
    {
        const EventType eventType = E::GetStaticEventType();
        DispatchQueue(eventType);
    }
}

#endif // SOGE_CORE_EVENTMANAGER_HPP
