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
public:
    void Cleanup();

    auto CreateScene(SceneLoader const& loadFunction, SceneUnloader const& unloadFunction, std::string const& sceneName) -> Scene&;
    void LoadScene(Scene& scene);
    void LoadScene(std::string const& sceneName);
    void UnloadActiveScene() const;
    [[nodiscard]] auto GetActiveScene() const -> Scene*;

    void LoadNewScene();

private:
    std::vector<std::unique_ptr<Scene>> m_scenes;
    Scene* m_currentScene{};
    Scene* m_nextScene{};
};

}  // namespace gla

#endif  // GALENA_SCENEMANAGER_HPP