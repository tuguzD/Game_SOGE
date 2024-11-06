#include "sogepch.hpp"

#include "SOGE/Core/EventManager.hpp"


namespace soge
{
    EventType EventManager::AnyEvent::GetEventType() const
    {
        return m_data.get<Event>().GetEventType();
    }

    bool EventManager::AnyEvent::IsHandled() const
    {
        return m_data.get<Event>().IsHandled();
    }

    EventManager::FunctionHandleInternal::FunctionHandleInternal(Parent&& aHandle, const EventType& aEventType) noexcept
        : Parent(std::move(aHandle)), m_eventType(aEventType)
    {
    }

    const EventType& EventManager::FunctionHandleInternal::GetEventType() const noexcept
    {
        return m_eventType;
    }

    bool EventManager::Remove(const FunctionHandle& aHandle)
    {
        const EventType eventType = aHandle.GetEventType();
        if (!m_eventQueue.ownsHandle(eventType, aHandle))
        {
            return false;
        }
        return m_eventQueue.removeListener(eventType, static_cast<FunctionHandle::Parent>(aHandle));
    }

    bool EventManager::Contains(const FunctionHandle& aHandle) const
    {
        const EventType eventType = aHandle.GetEventType();
        return m_eventQueue.ownsHandle(eventType, aHandle);
    }

    bool EventManager::IsQueueEmpty() const
    {
        return m_eventQueue.waitFor(std::chrono::nanoseconds(0));
    }

    bool EventManager::IsEmpty(const EventType& aEventType) const
    {
        return m_eventQueue.hasAnyListener(aEventType);
    }

    void EventManager::ClearQueue()
    {
        m_eventQueue.clearEvents();
    }

    void EventManager::Clear()
    {
        EventQueue eventQueue;
        m_eventQueue.swap(eventQueue);
    }

    void EventManager::DispatchQueue(const EventType& aEventType)
    {
        auto predictor = [&](const Event& aEvent) { return aEventType == aEvent.GetEventType(); };
        m_eventQueue.processIf(predictor);
    }

    void EventManager::DispatchQueue()
    {
        m_eventQueue.process();
    }
}
