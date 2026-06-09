#ifndef GALENA_COMPONENT_H
#define GALENA_COMPONENT_H

#include <cassert>
#include <concepts>
#include <cstdint>

namespace gla
{
class GameObject;

class Component
{
public:
    virtual ~Component() noexcept = default;

    Component(Component const&) = delete;
    auto operator=(Component const&) -> Component& = delete;
    Component(Component&&) = delete;
    auto operator=(Component&&) -> Component& = delete;

    GameObject* const m_pOwner;
protected:
    explicit Component(GameObject* pOwner)
        : m_pOwner(pOwner)
    {
    }

    bool m_active{ false };
    virtual void OnActivate() {}

    friend GameObject;
    virtual void Update() {};
    virtual void FixedUpdate() {};
    virtual void LateUpdate() {};
    virtual void LateFixedUpdate() {};
private:
    void Activate()
    {
        if (not m_active)
        {
            OnActivate();
            m_active = true;
        }
    }
};

template<typename ComponentType>
concept ComponentConcept = std::derived_from<ComponentType, Component>;

}  // namespace gla

#endif  // GALENA_COMPONENT_H
