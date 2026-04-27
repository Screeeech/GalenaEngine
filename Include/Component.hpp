#ifndef GALENA_COMPONENT_H
#define GALENA_COMPONENT_H

#include <concepts>

namespace gla
{
class GameObject;

class Component
{
public:
    virtual ~Component() noexcept = default;
    Component(Component&&) = delete;
    Component(Component const&) = delete;
    Component& operator=(Component&&) = delete;
    Component& operator=(Component const&) = delete;

    virtual void Update(float deltaTime) = 0;

protected:
    explicit Component(GameObject* pOwner)
        : m_pOwner(pOwner)
    {
    }

    GameObject* const m_pOwner;
};

template<typename ComponentType>
concept ComponentConcept = std::derived_from<ComponentType, Component>;

}  // namespace gla

#endif  // GALENA_COMPONENT_H
