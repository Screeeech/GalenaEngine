#include "SceneManager.hpp"

#include "Scene.hpp"

namespace gla
{

void SceneManager::Update(float deltaTime) const
{
    if (not m_currentScene)
        return;

    m_currentScene->Update(deltaTime);
}

void SceneManager::Render() const
{
    if (not m_currentScene)
        return;

    m_currentScene->Render();
}

void SceneManager::DrawUI() const
{
    if (not m_currentScene)
        return;

    m_currentScene->DrawUI();
}

void SceneManager::LoadScene(Scene* scene)
{
    // Check if scene exists
    // If it does store a reference in m_currentScene
    for (auto& pScene : m_scenes)
    {
        if (auto* sceneRef = pScene.get(); sceneRef == scene)
            m_currentScene = sceneRef;
    }
}

Scene* SceneManager::GetActiveScene() const
{
    return m_currentScene;
}

Scene& SceneManager::CreateScene()
{
    m_scenes.emplace_back(new Scene());
    return *m_scenes.back();
}

void SceneManager::RegisterRenderComponent(Renderable* component) const
{
    if (not m_currentScene)
        return;

    m_currentScene->RegisterRenderComponent(component);
}

void SceneManager::UnregisterRenderComponent(Renderable* component) const
{
    if (not m_currentScene)
        return;

    m_currentScene->UnregisterRenderComponent(component);
}

void SceneManager::RegisterUIComponent(UIComponent* component) const
{
    if (not m_currentScene)
        return;

    m_currentScene->RegisterUIComponent(component);
}

void SceneManager::UnregisterUIComponent(UIComponent* component) const
{
    if (not m_currentScene)
        return;

    m_currentScene->UnregisterUIComponent(component);
}


void SceneManager::Cleanup()
{
    m_scenes.clear();
    m_currentScene = nullptr;
}

void SceneManager::SortCachedRenderComponents()
{
    if (not m_currentScene)
        return;

    m_currentScene->SortCachedRenderComponents();
}

}  // namespace gla