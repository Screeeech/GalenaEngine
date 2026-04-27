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
    void RemoveGameObject(GameObject* pObject);
    void Update(float deltaTime);
    void Render();
    void DrawUI();
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

    [[nodiscard]] auto GetRoot() -> GameObject*;
private:
    friend class SceneManager;
    explicit Scene();

    std::unique_ptr<GameObject> m_pRootObject;
    std::vector<Renderable*> m_renderComponents;
    std::vector<UIComponent*> m_uiComponents;
};

}  // namespace gla

#endif // GALENA_SCENE_HPP