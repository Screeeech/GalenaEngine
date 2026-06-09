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

    ~Renderable() override
    {
        m_pOwner->GetParentScene().UnregisterRenderComponent(this);
    }

    void SetZIndex(int zIndex)
    {
        m_zIndex = zIndex;
        m_pOwner->GetParentScene().SortCachedRenderComponents();
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
        m_pOwner->GetParentScene().RegisterRenderComponent(this);
    }

private:
    int m_zIndex;
};

}  // namespace dae

#endif  // GALENA_RENDERABLE_H
