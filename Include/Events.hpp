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

struct EntityEvent : Event
{
    explicit EntityEvent(EventID id, int playerIndex)
        : Event(id)
        , entityIndex{ playerIndex }
    {
    }

    int entityIndex;
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

struct PlayerConnectionEvent final : EntityEvent
{
    explicit PlayerConnectionEvent(EventID id, int playerIndex, bool isGamepad)
        : EntityEvent(id, playerIndex)
        , isGamepad(isGamepad)
    {
    }

    bool isGamepad;
};

}  // namespace gla

#endif  // GALENA_EVENTS_H
