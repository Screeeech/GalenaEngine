#ifndef GALENA_RENDERABLE_H
#define GALENA_RENDERABLE_H

#include "Component.hpp"
#include "Locator.hpp"
#include "Services/SceneManager.hpp"

namespace gla
{

class Renderable : public Component
{
public:
    explicit Renderable(GameObject* pOwner, int zIndex)
        : Component(pOwner)
        , m_zIndex(zIndex)
    {
    }

    void SetZIndex(int zIndex)
    {
        m_zIndex = zIndex;
        Locator::Get<SceneManager>().SortCachedRenderComponents();
    }

    [[nodiscard]] auto GetZIndex() const -> int
    {
        return m_zIndex;
    }

protected:
    friend class Scene;
    virtual void Render() = 0;

    void OnActivate() override
    {
        // TODO: Change to Initialize and to parented scene instead of active scene
        Locator::Get<SceneManager>().GetActiveScene()->RegisterRenderComponent(this);
    }

    void OnDeactivate() override
    {
        Locator::Get<SceneManager>().UnregisterRenderComponent(this);
    }
private:
    int m_zIndex;
};

}  // namespace dae

#endif  // GALENA_RENDERABLE_H
