#include "Services/SceneManager.hpp"

#include "GameObject.hpp"
#include "Scene.hpp"

namespace gla
{

void SceneManager::Update() const
{
    if (not m_currentScene)
        return;

    m_currentScene->Update();
}

void SceneManager::FixedUpdate() const
{
    if (not m_currentScene)
        return;

    m_currentScene->FixedUpdate();
}

void SceneManager::LateUpdate() const
{
    if (not m_currentScene)
        return;

    m_currentScene->LateUpdate();
}

void SceneManager::LateFixedUpdate() const
{
    if (not m_currentScene)
        return;

    m_currentScene->LateFixedUpdate();
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
        {
            if (m_currentScene)
                m_currentScene->Unload();
            m_currentScene = sceneRef;
        }
    }
}

auto SceneManager::GetActiveScene() const -> Scene*
{
    return m_currentScene;
}

auto SceneManager::CreateScene() -> Scene&
{
    return *m_scenes.emplace_back(new Scene());
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

void SceneManager::SortCachedRenderComponents() const
{
    if (not m_currentScene)
        return;

    m_currentScene->SortCachedRenderComponents();
}

void SceneManager::ExecuteReparentingQueue() const
{
    if (not m_currentScene)
        return;

    m_currentScene->ExecuteReparentingQueue();
}

}  // namespace gla