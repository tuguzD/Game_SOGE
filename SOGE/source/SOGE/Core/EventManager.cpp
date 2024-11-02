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

    bool EventManager::IsEmpty() const
    {
        return m_eventQueue.emptyQueue();
    }

    void EventManager::Clear()
    {
        m_eventQueue.clearEvents();
    }
}
