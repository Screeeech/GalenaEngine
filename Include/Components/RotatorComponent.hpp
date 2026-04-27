#ifndef GALENA_ROTATORCOMPONENT_H
#define GALENA_ROTATORCOMPONENT_H
#include "Component.hpp"

namespace gla
{

class RotatorComponent : public Component
{
public:
    RotatorComponent(GameObject* pOwner, float rotationSpeed);
    ~RotatorComponent() override = default;

    RotatorComponent(RotatorComponent const&) = delete;
    auto operator=(RotatorComponent const&) -> RotatorComponent = delete;
    RotatorComponent(RotatorComponent&&) = delete;
    auto operator=(RotatorComponent&&) -> RotatorComponent = delete;

    void Update(float deltaTime) override;

private:
    float m_rotationSpeed;
    float m_angle{};
};

}  // namespace gla

#endif  // GALENA_ROTATORCOMPONENT_H
