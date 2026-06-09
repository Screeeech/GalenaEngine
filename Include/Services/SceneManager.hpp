#ifndef GALENA_SCENEMANAGER_HPP
#define GALENA_SCENEMANAGER_HPP

#include <functional>
#include <memory>
#include <vector>

#include "GameObject.hpp"
#include "Scene.hpp"

namespace gla
{

class UIComponent;
class Scene;
class Renderable;

class SceneManager final
{
    static constexpr auto persistentSceneName{ "__persistent" };

public:
    explicit SceneManager();

    auto CreateScene(
        std::string const& sceneName,
        std::optional<SceneLoader> const& loadFunction = std::nullopt,
        std::optional<SceneUnloader> const& unloadFunction = std::nullopt) -> Scene&;
    void LoadScene(Scene& scene);
    void LoadScene(std::string const& sceneName);
    void UnloadActiveScene() const;
    [[nodiscard]] auto GetActiveScene() const -> Scene*;

    void LoadNewScene();

    [[nodiscard]] auto GetPersistentScene() const -> Scene&;

private:
    std::vector<std::unique_ptr<Scene>> m_scenes;
    std::unique_ptr<Scene> m_persistentScene;
    Scene* m_currentScene{};
    Scene* m_nextScene{};
};

}  // namespace gla

#endif  // GALENA_SCENEMANAGER_HPP