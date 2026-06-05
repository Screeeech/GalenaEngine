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
    virtual ~Component() noexcept
    {
        // I am not a big fan of this approach, but calling virtual functions in destructors doesn't work very well.
        // So using RAII to cleanup components is not as easy if I want the lifetime of my components to live beyond the lifetime
        // of the object itself.
        // So the owning GameObject will be responsible for cleaning up all of it's components by calling Deactivate()
        assert(not m_active and "Component must be in an inactive state when being destroyed");
    }

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
    virtual void OnDeactivate() {}


    // I prefer not being able to call Update on my components from any other place, other than the GameObject updating them
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

    void Deactivate()
    {
        if (m_active)
        {
            OnDeactivate();
            m_active = false;
        }
    }

};

template<typename ComponentType>
concept ComponentConcept = std::derived_from<ComponentType, Component>;

}  // namespace gla

#endif  // GALENA_COMPONENT_H
