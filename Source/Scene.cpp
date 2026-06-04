#include "Scene.hpp"

#include <algorithm>
#include <cassert>

#include "Components/UIComponent.hpp"
#include "GameObject.hpp"
#include "Renderable.hpp"

namespace gla
{

void Scene::RemoveGameObject(GameObject* pObject) const
{
    assert(pObject != nullptr && "pObject is nullptr");
    m_pRootObject->RemoveChild(pObject);
}

void Scene::Update(float deltaTime) const
{
    m_pRootObject->Update(deltaTime);
}

void Scene::FixedUpdate(float deltaTime) const
{
    m_pRootObject->FixedUpdate(deltaTime);
}

void Scene::LateUpdate(float deltaTime) const
{
    m_pRootObject->LateUpdate(deltaTime);
}

void Scene::Render() const
{
    for (auto* renderComponents : m_renderComponents)
    {
        renderComponents->Render();
    }
}

void Scene::DrawUI() const
{
    for (const auto* uiComponent : m_uiComponents)
    {
        uiComponent->DrawUI();
    }
}

void Scene::Load()
{
    Locator::Get<SceneManager>().LoadScene(this);
    m_pRootObject->Activate();
}

void Scene::Unload() const
{
    m_pRootObject->Deactivate();
}


auto Scene::IsActive() const -> bool
{
    return Locator::Get<SceneManager>().GetActiveScene() == this;
}

void Scene::QueueReparent(GameObject& child, GameObject& newParent, bool keepWorldPosition)
{
    m_reparentQueue.emplace(child, newParent, keepWorldPosition);
}

void Scene::ExecuteReparentingQueue()
{
    while(not m_reparentQueue.empty())
    {
        auto const& [child, newParent, keepWorldPosition] = m_reparentQueue.front();

        child.Reparent(newParent, keepWorldPosition);

        m_reparentQueue.pop();
    }
}

void Scene::RegisterRenderComponent(Renderable* renderable)
{
    m_renderComponents.push_back(renderable);
    SortCachedRenderComponents();
}

void Scene::UnregisterRenderComponent(Renderable* component)
{
    if (not m_renderComponents.empty())
        std::erase(m_renderComponents, component);
}

void Scene::RegisterUIComponent(UIComponent* component)
{
    m_uiComponents.push_back(component);
}

void Scene::UnregisterUIComponent(UIComponent* component)
{
    std::erase(m_uiComponents, component);
}

void Scene::SortCachedRenderComponents()
{
    std::ranges::sort(
        m_renderComponents,
        [](Renderable const* pComp1, Renderable const* pComp2) { return pComp1->GetZIndex() < pComp2->GetZIndex(); });
}

auto Scene::GetRoot() const -> GameObject*
{
    return m_pRootObject.get();
}

auto Scene::operator==(const Scene& other) const -> bool
{
    return other.m_pRootObject.get() == m_pRootObject.get();
}

Scene::Scene()
    : m_pRootObject(new GameObject(*this, 0, 0, "Scene root"))
{
}


}  // namespace gla