#ifndef GALENA_EVENTMANAGER_H
#define GALENA_EVENTMANAGER_H

#include <any>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <ranges>
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

    template<typename ListenerType, typename MemFn>
    void BindEvent(EventID id, ListenerType* listener, MemFn callback)
    {
        m_listeners.emplace(id, EventEntry{ listener, std::bind_front(callback, listener), false });
    }

    template<typename ListenerType>
    void UnbindEvents(ListenerType* listener)
    {
        // std::erase_if(m_listeners, [&](auto const& pair) { return pair.second.first == listener; });
        for (auto& [storedListener, callback, unbindFlag] : m_listeners | std::views::values)
        {
            if (storedListener == listener)
                unbindFlag = true;
        }

        for (auto& [eventArgs, eventID, entry] : m_queuedEvents)
        {
            auto& [storedListener, callback, unbindFlag] = entry;
            if (storedListener == listener)
                unbindFlag = true;
        }
    }

    template<typename ListenerType>
    void UnbindEvent(EventID id, ListenerType* listener)
    {
        // std::erase_if(m_listeners, [&](auto const& pair) { return pair.first == id and pair.second.first == listener; });
        // clang-format off
        for (auto const [fst, snd] = m_listeners.equal_range(id);
             auto& [eventID, entry] : std::ranges::subrange(fst, snd))
        {
            auto& [storedListener, callback, unbindFlag] = entry;
            if (storedListener == listener and eventID == id)
                unbindFlag = true;
        }

        for (auto&& [eventArgs, eventID, entry] : m_queuedEvents)
        {
            auto& [storedListener, callback, unbindFlag] = entry;
            if (storedListener == listener and eventID == id)
                unbindFlag = true;
        }
        // clang-format on
    }

    void InvokeEvent(EventID eventID, std::any const& eventArgs);

    template<typename EventType>
        requires std::derived_from<EventType, Event>
    void InvokeEvent(EventType const& eventArgs)
    {
        auto range = m_listeners.equal_range(eventArgs.eventID);
        auto subrange = std::ranges::subrange(range.first, range.second);
        for (auto&& [listener, callback, unbindFlag] : subrange | std::views::values)
        {
            if (not unbindFlag)
                callback(std::make_any<EventType>(eventArgs));
        }
    }

    template<typename EventType>
        requires std::derived_from<EventType, Event>
    void QueueEvent(EventType const& eventArgs)
    {
        auto range = m_listeners.equal_range(eventArgs.eventID);
        for (auto& entry : std::ranges::subrange(range.first, range.second) | std::views::values)
            if (not entry.unbindFlag)
                m_queuedEvents.emplace_back(std::make_any<EventType>(eventArgs), eventArgs.eventID, entry);
    }

    void EraseFlaggedEventBindings();
    void ExecuteQueuedEvents();
    void FlushQueuedEvents();

private:
    struct EventEntry final
    {
        void* listener;
        EventCallback callback;
        bool unbindFlag;
    };

    struct EventQueueEntry final
    {
        std::any eventArgs;
        EventID id;
        EventEntry entry;
    };

    // What if object gets deleted before event can fire?
    std::deque<EventQueueEntry> m_queuedEvents;

    std::unordered_multimap<EventID, EventEntry> m_listeners;
};

}  // namespace gla

#endif  // GALENA_EVENTMANAGER_H
