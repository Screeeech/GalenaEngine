#include "Services/EventManager.hpp"

#include "Events.hpp"

namespace gla
{

EventManager::EventManager(EventManager const& /*other*/)
{
    // TODO: Again this is very bad, I need to figure out how to store services in a way that allows them to not have a copy constructor
    throw std::runtime_error("Cannot copy construct InputManager, no clue how to fix this yet");
}

void EventManager::InvokeEvent(const Event& event)
{
    auto range = m_listeners.equal_range(event.eventID);
    for(auto&& [key, value] : std::ranges::subrange(range.first, range.second))
    {
        value.second(event);
    }
}

void EventManager::ExecuteQueuedEvents()
{
    while(not m_queuedEvents.empty())
    {
        auto& event = m_queuedEvents.front().first;
        auto callback = m_queuedEvents.front().second;
        callback(*event);
        m_queuedEvents.pop();
    }
}

}  // namespace gla