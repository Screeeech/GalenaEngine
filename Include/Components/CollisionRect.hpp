#ifndef GALENA_COLLISIONRECT_HPP
#define GALENA_COLLISIONRECT_HPP

#include <glm/vec2.hpp>

#include "Collider.hpp"
#include "Component.hpp"

namespace gla
{

class CollisionRect final : public Collider
{
public:
    template<typename EventType>
        requires std::derived_from<EventType, Event>
    CollisionRect(
        GameObject* pOwner,
        uint32_t collisionLayersBits,
        uint32_t collisionMasksBits,
        EventType const& eventArgs,
        glm::vec2 position,
        glm::vec2 size,
        bool active = true)
        : Collider(pOwner, collisionLayersBits, collisionMasksBits, eventArgs, active)
        , m_position(position)
        , m_size(size)
    {
    }
    explicit CollisionRect(
        GameObject* pOwner,
        uint32_t collisionLayersBits,
        uint32_t collisionMasksBits,
        CollisionCallback const& callback,
        glm::vec2 position,
        glm::vec2 size,
        bool active = true);
    ~CollisionRect() override;

    glm::vec2 m_position;
    glm::vec2 m_size;

protected:
    void OnActivate() override;
    void FixedUpdate() override;
    void Render() override;

private:
    [[nodiscard]] auto Left() const -> float;
    [[nodiscard]] auto Right() const -> float;
    [[nodiscard]] auto Bottom() const -> float;
    [[nodiscard]] auto Top() const -> float;


    static std::vector<CollisionRect*> m_registeredColliders;

    [[nodiscard]] auto IsColliding(CollisionRect const& collider) const -> bool;
};

}  // namespace gla

#endif
