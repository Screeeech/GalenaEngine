#ifndef GALENA_SCENE_HPP
#define GALENA_SCENE_HPP

#include <memory>
#include <vector>

#include "GameObject.hpp"

namespace gla
{

class UIComponent;
class Renderable;

class Scene final
{
public:
    void RemoveGameObject(GameObject* pObject) const;
    void Update(float deltaTime) const;
    void FixedUpdate(float deltaTime) const;
    void Render() const;
    void DrawUI() const;
    void Load();
    void RegisterRenderComponent(Renderable* renderable);
    void UnregisterRenderComponent(Renderable* component);
    void RegisterUIComponent(UIComponent* component);
    void UnregisterUIComponent(UIComponent* component);
    void SortCachedRenderComponents();

    ~Scene() = default;
    Scene(Scene const& other) = delete;
    auto operator=(Scene const& other) -> Scene& = delete;
    Scene(Scene&& other) = delete;
    auto operator=(Scene&& other) -> Scene& = delete;

    [[nodiscard]] auto GetRoot() const -> GameObject*;
private:
    friend class SceneManager;
    explicit Scene();

    std::unique_ptr<GameObject> m_pRootObject;
    std::vector<Renderable*> m_renderComponents;
    std::vector<UIComponent*> m_uiComponents;
};

}  // namespace gla

#endif // GALENA_SCENE_HPP