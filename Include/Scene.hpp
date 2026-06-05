#ifndef GALENA_SCENE_HPP
#define GALENA_SCENE_HPP

#include <memory>
#include <queue>
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
    void Update() const;
    void FixedUpdate() const;
    void LateFixedUpdate() const;
    void LateUpdate() const;
    void Render() const;
    void DrawUI() const;

    void Load();
    void Unload() const;

    auto IsActive() const -> bool;

    void QueueReparent(GameObject& child, GameObject& newParent, bool keepWorldPosition);
    void ExecuteReparentingQueue();

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

    struct ReparentOperation final
    {
        GameObject& child;
        GameObject& newParent;
        bool keepWorldPosition;
    };

    std::queue<ReparentOperation> m_reparentQueue;
};

}  // namespace gla

#endif  // GALENA_SCENE_HPP