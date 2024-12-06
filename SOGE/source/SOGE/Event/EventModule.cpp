#include "sogepch.hpp"

#include "SOGE/Event/EventModule.hpp"


namespace soge
{
    EventType EventModule::AnyEvent::GetEventType() const
    {
        return m_data.get<Event>().GetEventType();
    }

    bool EventModule::AnyEvent::IsHandled() const
    {
        return m_data.get<Event>().IsHandled();
    }

    EventModule::FunctionHandle::FunctionHandle(Parent&& aHandle, const EventType& aEventType) noexcept
        : Parent(std::move(aHandle)), m_eventType(aEventType)
    {
    }

    const EventType& EventModule::FunctionHandle::GetEventType() const noexcept
    {
        return m_eventType;
    }

    bool EventModule::Remove(const FunctionHandle& aHandle)
    {
        const EventType eventType = aHandle.GetEventType();
        if (!m_eventQueue.ownsHandle(eventType, aHandle))
        {
            return false;
        }
        return m_eventQueue.removeListener(eventType, static_cast<FunctionHandle::Parent>(aHandle));
    }

    bool EventModule::Contains(const FunctionHandle& aHandle) const
    {
        const EventType eventType = aHandle.GetEventType();
        return m_eventQueue.ownsHandle(eventType, aHandle);
    }

    bool EventModule::IsQueueEmpty() const
    {
        return m_eventQueue.waitFor(std::chrono::nanoseconds(0));
    }

    bool EventModule::IsEmpty(const EventType& aEventType) const
    {
        return m_eventQueue.hasAnyListener(aEventType);
    }

    void EventModule::ClearQueue()
    {
        m_eventQueue.clearEvents();
    }

    void EventModule::Clear()
    {
        EventQueue eventQueue;
        m_eventQueue.swap(eventQueue);
    }

    void EventModule::DispatchQueue(const EventType& aEventType)
    {
        auto predictor = [&](const Event& aEvent) { return aEventType == aEvent.GetEventType(); };
        m_eventQueue.processIf(predictor);
    }

    void EventModule::DispatchQueue()
    {
        m_eventQueue.process();
    }

    void EventModule::Load(di::Container& aContainer)
    {
    }

    void EventModule::Unload(di::Container& aContainer)
    {
    }
}
