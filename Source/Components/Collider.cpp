#include "Components/Collider.hpp"

#include <algorithm>
#include <ranges>

#include "Components/Animation.hpp"
#include "Services/EventManager.hpp"

namespace gla
{

Collider::Collider(GameObject* pOwner, uint32_t collisionLayersBits, uint32_t collisionMasksBits, EventID eventID, bool active)
    : Renderable(pOwner, 5)
    , m_collisionLayers(collisionLayersBits)
    , m_collisionMasks(collisionMasksBits)
    , m_active(active)
    , m_trigger(eventID)
{
}

Collider::Collider(GameObject* pOwner, uint32_t collisionLayersBits, uint32_t collisionMasksBits, CollisionCallback const& callback, bool active)
    : Renderable(pOwner, 5)
    , m_collisionLayers(collisionLayersBits)
    , m_collisionMasks(collisionMasksBits)
    , m_active(active)
    , m_trigger(callback)
{
}

void Collider::SetCollisionLayer(uint32_t layers)
{
    m_collisionLayers = layers;
}
void Collider::CombineCollisionLayer(uint32_t layers)
{
    m_collisionLayers |= layers;
}
void Collider::ClearCollisionLayer(uint32_t layers)
{
    m_collisionLayers &= ~layers;
}
void Collider::SetCollisionMask(uint32_t masks)
{
    m_collisionMasks = masks;
}
void Collider::CombineCollisionMask(uint32_t masks)
{
    m_collisionMasks |= masks;
}
void Collider::ClearCollisionMask(uint32_t masks)
{
    m_collisionMasks &= ~masks;
}

void Collider::Collide(Collider& collider, Collider& other) const
{
    if (std::holds_alternative<EventID>(m_trigger))
        Locator::Get<EventManager>().InvokeEvent(CollisionEvent{ std::get<EventID>(m_trigger), &collider, &other });
    else
        std::get<CollisionCallback>(m_trigger)(collider, other);
}

void Collider::Enable()
{
    m_active = true;
}

void Collider::Disable()
{
    m_active = false;
}

void Collider::OnActivate()
{
    Renderable::OnActivate();
}
void Collider::OnDeactivate()
{
    Renderable::OnDeactivate();
}

}  // namespace gla
