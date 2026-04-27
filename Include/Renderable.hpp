#ifndef GALENA_RENDERABLE_H
#define GALENA_RENDERABLE_H

#include "Component.hpp"
#include "SceneManager.hpp"

namespace gla
{

class Renderable : public Component
{
public:
    explicit Renderable(GameObject* pOwner, int zIndex)
        : Component(pOwner)
        , m_zIndex(zIndex)
    {
        // TODO: Change to Initialize and to parented scene instead of active scene
        SceneManager::Get().GetActiveScene()->RegisterRenderComponent(this);
    }

    Renderable(Renderable const&) = delete;
    auto operator=(Renderable const&) -> Renderable& = delete;
    Renderable(Renderable&&) = delete;
    auto operator=(Renderable&&) -> Renderable& = delete;

    ~Renderable() noexcept override
    {
        SceneManager::Get().UnregisterRenderComponent(this);
    }

    virtual void Render() = 0;

    void SetZIndex(int zIndex)
    {
        m_zIndex = zIndex;
        SceneManager::Get().SortCachedRenderComponents();
    }

    [[nodiscard]] auto GetZIndex() const -> int
    {
        return m_zIndex;
    }

private:
    int m_zIndex;
};

}  // namespace dae

#endif  // GALENA_RENDERABLE_H
