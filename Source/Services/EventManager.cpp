#include "Services/EventManager.hpp"

#include "Events.hpp"

namespace gla
{

void EventManager::ExecuteQueuedEvents()
{
    while(not m_queuedEvents.empty())
    {
        auto const& event = m_queuedEvents.front().first;
        auto callback = m_queuedEvents.front().second;
        callback(event);
        m_queuedEvents.pop();
    }
}

}  // namespace gla