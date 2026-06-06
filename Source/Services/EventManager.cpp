#include "Services/EventManager.hpp"

#include "Events.hpp"

namespace gla
{

void EventManager::InvokeEvent(EventID eventID, std::any const& eventArgs)
{
    auto range = m_listeners.equal_range(eventID);
    auto subrange = std::ranges::subrange(range.first, range.second);
    for (auto&& [listener, callback, unbindFlag] : subrange | std::views::values)
    {
        if (not unbindFlag)
            callback(eventArgs);
    }
}

void EventManager::EraseFlaggedEventBindings()
{
    std::erase_if(m_queuedEvents, [](EventQueueEntry const& queueEntry) -> bool { return queueEntry.entry->unbindFlag; });
    std::erase_if(m_listeners, [](std::pair<EventID, EventEntry> const& pair) -> bool { return pair.second.unbindFlag; });
}

void EventManager::ExecuteQueuedEvents()
{
    while (not m_queuedEvents.empty())
    {
        auto& [eventArgs, eventID, entry] = m_queuedEvents.front();
        if (not entry->unbindFlag)
            entry->callback(eventArgs);

        m_queuedEvents.pop_front();
    }
}

}  // namespace gla