#include "Components/Collider.hpp"

#include <algorithm>
#include <ranges>

#include "Components/Animation.hpp"

namespace gla
{

Collider::Collider(GameObject* pOwner, uint32_t collisionLayersBits, uint32_t collisionMasksBits, std::vector<CollisionCallback>&& callbacks, bool active)
    : Renderable(pOwner, 5)
    , m_active(active)
    , m_collisionLayers(collisionLayersBits)
    , m_collisionMasks(collisionMasksBits)
    , m_callbacks(std::move(callbacks))
{
}

void Collider::Collide(Collider const& collider) const
{
    for (auto const& callback : m_callbacks)
        callback(collider);
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
