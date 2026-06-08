#include "Services/SceneManager.hpp"

#include <print>

#include "GameObject.hpp"
#include "Scene.hpp"

namespace gla
{

auto SceneManager::CreateScene(SceneLoader const& loadFunction, SceneUnloader const& unloadFunction, std::string const& sceneName) -> Scene&
{
    return *m_scenes.emplace_back(new Scene(loadFunction, unloadFunction, sceneName));
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

void SceneManager::Cleanup()
{
    m_scenes.clear();
    m_currentScene = nullptr;
}

}  // namespace gla