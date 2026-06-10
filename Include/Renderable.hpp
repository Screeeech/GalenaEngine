#ifndef GALENA_RENDERABLE_H
#define GALENA_RENDERABLE_H

#include "Component.hpp"
#include "Locator.hpp"
#include "Services/Renderer.hpp"
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
        Locator::Get<Renderer>().SortCachedRenderables();
    }

    [[nodiscard]] auto GetZIndex() const -> int
    {
        return m_zIndex;
    }

protected:
    friend class Renderer;
    virtual void Render() = 0;

    void OnActivate() override
    {
        Locator::Get<Renderer>().RegisterRenderable(this);
    }

    void OnDeactivate() override
    {
        Locator::Get<Renderer>().UnregisterRenderable(this);
    }
private:
    int m_zIndex;
};

}  // namespace dae

#endif  // GALENA_RENDERABLE_H
