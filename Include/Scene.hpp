#ifndef GALENA_SCENE_HPP
#define GALENA_SCENE_HPP

#include <memory>
#include <vector>

namespace gla
{
class GameObject;

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
    void Unload() const;

    auto IsActive() const -> bool;
    void RegisterRenderComponent(Renderable* renderable);
    void UnregisterRenderComponent(Renderable* component);
    void RegisterUIComponent(UIComponent* component);
    void UnregisterUIComponent(UIComponent* component);
    void SortCachedRenderComponents();

    [[nodiscard]] auto GetRoot() const -> GameObject*;

    auto operator==(const Scene& other) const -> bool;

private:
    friend class SceneManager;
    explicit Scene();

    std::unique_ptr<GameObject> m_pRootObject;
    std::vector<Renderable*> m_renderComponents;
    std::vector<UIComponent*> m_uiComponents;
};

}  // namespace gla

#endif  // GALENA_SCENE_HPP