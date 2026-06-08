#include "Services/SceneManager.hpp"

#include "GameObject.hpp"
#include "Scene.hpp"

namespace gla
{

auto SceneManager::CreateScene(SceneLoader const& loadFunction, std::string const& sceneName) -> Scene&
{
    return *m_scenes.emplace_back(new Scene(loadFunction, sceneName));
}

void SceneManager::LoadScene(Scene& scene)
{
    if (m_currentScene == &scene)
        return;

    if (m_currentScene)
        m_currentScene->Unload();

    m_currentScene = &scene;
    m_currentScene->Load();
}

void SceneManager::UnloadActiveScene() const
{
    if (m_currentScene)
        m_currentScene->Unload();
}

auto SceneManager::GetActiveScene() const -> Scene*
{
    return m_currentScene;
}

void SceneManager::Cleanup()
{
    m_scenes.clear();
    m_currentScene = nullptr;
}

}  // namespace gla