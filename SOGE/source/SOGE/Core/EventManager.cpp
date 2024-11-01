#include "sogepch.hpp"

#include "SOGE/Core/EventManager.hpp"


namespace soge
{
    bool EventManager::IsEmpty() const
    {
        return m_eventQueue.emptyQueue();
    }

    void EventManager::Clear()
    {
        m_eventQueue.clearEvents();
    }
}
