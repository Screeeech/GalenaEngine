#include "Services/SceneManager.hpp"

#include <print>

#include "GameObject.hpp"
#include "Scene.hpp"

namespace gla
{

SceneManager::SceneManager()
    : m_persistentScene(new Scene(persistentSceneName))
{
    m_persistentScene->Load();
}

auto SceneManager::CreateScene(
    std::string const& sceneName, std::optional<SceneLoader> const& loadFunction, std::optional<SceneUnloader> const& unloadFunction) -> Scene&
{
    return *m_scenes.emplace_back(new Scene(sceneName, loadFunction, unloadFunction));
}

void SceneManager::LoadScene(Scene& scene)
{
    if (m_currentScene == &scene)
        return;

    m_nextScene = &scene;
}

void SceneManager::LoadScene(std::string const& sceneName)
{
    for (auto const& pScene : m_scenes)
    {
        if (pScene->m_sceneName == sceneName)
        {
            LoadScene(*pScene);
            return;
        }
    }

    std::println("Warning, no scene with name {} found", sceneName);
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

void SceneManager::LoadNewScene()
{
    if (m_nextScene)
    {
        if (m_currentScene)
            m_currentScene->Unload();

        m_currentScene = m_nextScene;
        m_currentScene->Load();

        m_nextScene = nullptr;
    }
}

auto SceneManager::GetPersistentScene() const -> Scene&
{
    return *m_persistentScene;
}

}  // namespace gla