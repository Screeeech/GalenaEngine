#ifndef GALENA_COLLIDER_HPP
#define GALENA_COLLIDER_HPP

#include <functional>
#include <vector>

#include "Renderable.hpp"

namespace gla
{

class Collider;
using CollisionCallback = std::function<void(Collider const&)>;

class Collider : public Renderable
{
public:
    explicit Collider(GameObject* pOwner, uint32_t collisionLayersBits, uint32_t collisionMasksBits, std::vector<CollisionCallback>&& callbacks);

    void Collide(Collider const& other) const;

    enum class Bits : uint32_t
    {
        Layer1 = 1U << 0,
        Layer2 = 1U << 1,
        Layer3 = 1U << 2,
        Layer4 = 1U << 3,
        Layer5 = 1U << 4,
        Layer6 = 1U << 5,
        Layer7 = 1U << 6,
        Layer8 = 1U << 7,
        Layer9 = 1U << 8,
        Layer10 = 1U << 9,
        Layer11 = 1U << 10,
        Layer12 = 1U << 11,
        Layer13 = 1U << 12,
        Layer14 = 1U << 13,
        Layer15 = 1U << 14,
        Layer16 = 1U << 15,
        Layer17 = 1U << 16,
        Layer18 = 1U << 17,
        Layer19 = 1U << 18,
        Layer20 = 1U << 19,
        Layer21 = 1U << 20,
        Layer22 = 1U << 21,
        Layer23 = 1U << 22,
        Layer24 = 1U << 23,
        Layer25 = 1U << 24,
        Layer26 = 1U << 25,
        Layer27 = 1U << 26,
        Layer28 = 1U << 27,
        Layer29 = 1U << 28,
        Layer30 = 1U << 29,
        Layer31 = 1U << 30,
        Layer32 = 1U << 31,
    };

protected:
    void OnActivate() override;
    void OnDeactivate() override;

    uint32_t m_collisionLayers;
    uint32_t m_collisionMasks;
    std::vector<CollisionCallback> m_callbacks;
};

}  // namespace gla

#endif  // GALENA_COLLIDER_HPP
