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
    explicit CollisionRect(
        GameObject* pOwner,
        uint32_t collisionLayersBits,
        uint32_t collisionMasksBits,
        EventID eventID,
        glm::vec2 position,
        glm::vec2 size,
        bool active = true);
    explicit CollisionRect(
        GameObject* pOwner,
        uint32_t collisionLayersBits,
        uint32_t collisionMasksBits,
        CollisionCallback const& callback,
        glm::vec2 position,
        glm::vec2 size,
        bool active = true);

protected:
    void OnActivate() override;
    void OnDeactivate() override;
    void FixedUpdate(float fixedDeltaTime) override;
    void Render() override;

private:
    [[nodiscard]] auto Left() const -> float;
    [[nodiscard]] auto Right() const -> float;
    [[nodiscard]] auto Bottom() const -> float;
    [[nodiscard]] auto Top() const -> float;

    glm::vec2 m_position;
    glm::vec2 m_size;

    static std::vector<CollisionRect*> m_registeredColliders;

    [[nodiscard]] auto IsColliding(CollisionRect const& collider) const -> bool;
};

}  // namespace gla

#endif
