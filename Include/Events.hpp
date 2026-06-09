#ifndef GALENA_EVENTS_H
#define GALENA_EVENTS_H

#include <cstdint>

using EventID = uint32_t;

namespace gla
{
class Collider;

struct Event
{
    virtual ~Event() = default;

    explicit Event(EventID id)
        : eventID{ id }
    {
    }

    EventID eventID{};
};

struct PlayerEvent : Event
{
    explicit PlayerEvent(EventID id, int playerIndex)
        : Event(id)
        , playerIndex{ playerIndex }
    {
    }

    int playerIndex;
};

struct CollisionEvent : Event
{
    explicit CollisionEvent(EventID id, Collider* pCollider, Collider* pOtherCollider)
        : Event(id)
        , pCollider(pCollider)
        , pOtherCollider(pOtherCollider)
    {
    }

    Collider* pCollider;
    Collider* pOtherCollider;
};

struct PlayerConnectionEvent final : PlayerEvent
{
    explicit PlayerConnectionEvent(EventID id, int playerIndex, bool isGamepad)
        : PlayerEvent(id, playerIndex)
        , isGamepad(isGamepad)
    {
    }

    bool isGamepad;
};

}  // namespace gla

#endif  // GALENA_EVENTS_H
