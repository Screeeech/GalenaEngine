#include "Components/CollisionRect.hpp"

#include "../../../Include/Colors.hpp"
#include "GameObject.hpp"
#include "Services/Renderer.hpp"

namespace gla
{

std::vector<CollisionRect*> CollisionRect::m_registeredColliders;

CollisionRect::CollisionRect(
    GameObject* pOwner,
    uint32_t collisionLayersBits,
    uint32_t collisionMasksBits,
    std::vector<CollisionCallback>&& callbacks,
    glm::vec2 position,
    glm::vec2 size,
    bool active)
    : Collider(pOwner, collisionLayersBits, collisionMasksBits, std::move(callbacks), active)
    , m_position(position)
    , m_size(size)
{
}

void CollisionRect::OnActivate()
{
    Collider::OnActivate();

    m_registeredColliders.push_back(this);
}

void CollisionRect::OnDeactivate()
{
    Collider::OnDeactivate();

    std::erase_if(m_registeredColliders, [this](auto* other) -> bool { return other == this; });
}
void CollisionRect::FixedUpdate(float /*fixedDeltaTime*/)
{
    if (not m_active)
        return;

    for (auto const* collider : m_registeredColliders)
    {
        if (collider == this or not collider->m_active)
            continue;

        if (m_collisionMasks & collider->m_collisionLayers)
            if (IsColliding(*collider))
                collider->Collide(*this);
    }
}

void CollisionRect::Render()
{
    if (not m_active)
        return;

    auto const& renderer = Locator::Get<Renderer>();
    auto const worldPosition = m_pOwner->GetWorldPosition();

    renderer.SetColor(SDL_Color{ 255, 0, 0, 255 });
    renderer.DrawRect({ worldPosition.x + m_position.x, worldPosition.y + m_position.y, m_size.x, m_size.y });
}

auto CollisionRect::Left() const -> float
{
    return m_position.x + m_pOwner->GetWorldPosition().x;
}

auto CollisionRect::Right() const -> float
{
    return m_position.x + m_size.x + m_pOwner->GetWorldPosition().x;
}

auto CollisionRect::Bottom() const -> float
{
    return m_position.y + m_pOwner->GetWorldPosition().y;
}

auto CollisionRect::Top() const -> float
{
    return m_position.y + m_size.y + m_pOwner->GetWorldPosition().y;
}

auto CollisionRect::IsColliding(CollisionRect const& collider) const -> bool
{
    // clang-format off
    return Left()   < collider.Right()  and
           Right()  > collider.Left()   and
           Top()    > collider.Bottom() and
           Bottom() < collider.Top();
}


}  // namespace gla