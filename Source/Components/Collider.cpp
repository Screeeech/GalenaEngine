#include "Components/Collider.hpp"

#include <algorithm>
#include <ranges>

#include "Components/Animation.hpp"
#include "Services/EventManager.hpp"

namespace gla
{

Collider::Collider(GameObject* pOwner, uint32_t collisionLayersBits, uint32_t collisionMasksBits, CollisionCallback const& callback, bool active)
    : Renderable(pOwner, 5)
    , m_collisionLayers(collisionLayersBits)
    , m_collisionMasks(collisionMasksBits)
    , m_active(active)
    , m_trigger(callback)
{
}

void Collider::SetCollisionLayers(uint32_t layers)
{
    m_collisionLayers = layers;
}
void Collider::EnableCollisionLayers(uint32_t layers)
{
    m_collisionLayers |= layers;
}
void Collider::DisableCollisionLayers(uint32_t layers)
{
    m_collisionLayers &= ~layers;
}
void Collider::SetCollisionMasks(uint32_t masks)
{
    m_collisionMasks = masks;
}
void Collider::EnableCollisionMasks(uint32_t masks)
{
    m_collisionMasks |= masks;
}
void Collider::DisableCollisionMasks(uint32_t masks)
{
    m_collisionMasks &= ~masks;
}

void Collider::Collide(Collider& collider, Collider& other) const
{
    if (std::holds_alternative<EventPayload>(m_trigger))
    {
        auto const& [id, args] = std::get<EventPayload>(m_trigger);
        Locator::Get<EventManager>().InvokeEvent(id, args);
    }
    else
    {
        std::get<CollisionCallback>(m_trigger)(collider, other);
    }
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

}  // namespace gla
