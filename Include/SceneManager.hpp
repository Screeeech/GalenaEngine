#ifndef GALENA_SCENEMANAGER_HPP
#define GALENA_SCENEMANAGER_HPP

#include <memory>
#include <vector>

#include "Scene.hpp"
#include "Singleton.hpp"

namespace gla
{

class UIComponent;
class Scene;
class Renderable;

// NOTE: This isn't a service locator yet, because ideally I want to gut this class entirely
class SceneManager final : public Singleton<SceneManager>
{
public:
    auto CreateScene() -> Scene&;

    void Update(float deltaTime) const;
    void Render() const;
    void DrawUI() const;

    void LoadScene(Scene* scene);
    [[nodiscard]] auto GetActiveScene() const -> Scene*;
    void RegisterRenderComponent(Renderable* component) const;
    void UnregisterRenderComponent(Renderable* component) const;
    void RegisterUIComponent(UIComponent* component) const;
    void UnregisterUIComponent(UIComponent* component) const;

    void Cleanup();

    void SortCachedRenderComponents();

private:
    friend class Singleton<SceneManager>;
    std::vector<std::unique_ptr<Scene>> m_scenes;
    Scene* m_currentScene{};
};

}  // namespace gla

#endif // GALENA_SCENEMANAGER_HPP