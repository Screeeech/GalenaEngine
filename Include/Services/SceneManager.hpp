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

    auto CreateScene(SceneLoader const& loadFunction, std::string const& sceneName) -> Scene&;
    void LoadScene(Scene& scene);
    void UnloadActiveScene() const;
    [[nodiscard]] auto GetActiveScene() const -> Scene*;

private:
    std::vector<std::unique_ptr<Scene>> m_scenes;
    Scene* m_currentScene{};
};

}  // namespace gla

#endif  // GALENA_SCENEMANAGER_HPP