#ifndef GALENA_SCENE_HPP
#define GALENA_SCENE_HPP

#include <functional>
#include <memory>
#include <queue>
#include <vector>

namespace gla
{
class Scene;
class GameObject;
class UIComponent;
class Renderable;

using SceneLoader = std::function<void(Scene&)>;
using SceneUnloader = std::function<void()>;

class Scene final
{
    friend class SceneManager;
    explicit Scene(
        std::string sceneName,
        std::optional<SceneLoader> loadFunction = std::nullopt,
        std::optional<SceneUnloader> unloadFunction = std::nullopt);

    void Load();
    void Unload();

public:
    void RemoveGameObject(GameObject* pObject) const;


    [[nodiscard]] auto IsActive() const -> bool;

    void QueueReparent(GameObject& child, GameObject& newParent, bool keepWorldPosition);
    void ExecuteReparentingQueue();

    void RegisterRenderComponent(Renderable* renderable);
    void UnregisterRenderComponent(Renderable* component);
    void RegisterUIComponent(UIComponent* component);
    void UnregisterUIComponent(UIComponent* component);
    void SortCachedRenderComponents();

    [[nodiscard]] auto GetRoot() const -> GameObject*;

    void Update() const;
    void FixedUpdate() const;
    void LateFixedUpdate() const;
    void LateUpdate() const;
    void Render() const;
    void DrawUI() const;
    auto operator==(const Scene& other) const -> bool;

private:
    bool m_active{};
    std::optional<SceneLoader> m_loadFunction;
    std::optional<SceneUnloader> m_unloadFunction;
    std::string m_sceneName;

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