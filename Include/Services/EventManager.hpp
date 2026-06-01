#ifndef GALENA_EVENTMANAGER_H
#define GALENA_EVENTMANAGER_H

#include <any>
#include <cstdint>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

namespace gla
{
struct Event;
class EventListener;
class Observer;

using EventID = std::uint32_t;

using EventCallback = std::function<void(std::any)>;

class EventManager final

{
public:
    EventManager() = default;
    ~EventManager() = default;

    EventManager(EventManager const&) = delete;
    auto operator=(EventManager const&) -> EventManager& = delete;
    EventManager(EventManager&&) = delete;
    auto operator=(EventManager&&) -> EventManager& = delete;

    template<typename ListenerType>
    void BindEvent(EventID id, ListenerType* listener, void (ListenerType::*callback)(std::any const&))
    {
        m_listeners.emplace(id, std::pair{ listener, std::bind_front(callback, listener) });
    }

    template<typename ListenerType>
    void UnbindEvents(ListenerType* listener)
    {
        std::erase_if(m_listeners, [&](auto const& pair) { return pair.second.first == listener; });
    }

    template<typename ListenerType>
    void UnbindEvent(EventID id, ListenerType* listener)
    {
        std::erase_if(m_listeners, [&](auto const& pair) { return pair.first == id and pair.second.first == listener; });
    }

    template<typename EventType>
        requires std::derived_from<EventType, Event>
    void InvokeEvent(EventType const& eventArgs)
    {
        auto range = m_listeners.equal_range(eventArgs.eventID);
        for (auto&& [eventID, functionPair] : std::ranges::subrange(range.first, range.second))
            functionPair.second(std::make_any<EventType>(eventArgs));
    }

    template<typename EventType>
        requires std::derived_from<EventType, Event>
    void QueueEvent(EventType const& event)
    {
        auto range = m_listeners.equal_range(event.eventID);
        for (auto&& [evenID, functionPair] : std::ranges::subrange(range.first, range.second))
            m_queuedEvents.emplace(std::make_any<EventType>(event), functionPair.second);
    }

    void ExecuteQueuedEvents();

private:
    // What if object gets deleted before event can fire?
    std::queue<std::pair<std::any, EventCallback>> m_queuedEvents;

    std::unordered_multimap<EventID, std::pair<void*, EventCallback>> m_listeners;
};

}  // namespace gla

#endif  // GALENA_EVENTMANAGER_H
