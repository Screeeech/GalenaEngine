#include "Scene.hpp"

#include <algorithm>
#include <cassert>
#include <utility>

#include "Components/UIComponent.hpp"
#include "GameObject.hpp"
#include "Renderable.hpp"

namespace gla
{

Scene::Scene(std::string sceneName, std::optional<SceneLoader> loadFunction, std::optional<SceneUnloader> unloadFunction)
    : m_initFunction(std::move(loadFunction))
    , m_unloadFunction(std::move(unloadFunction))
    , m_sceneName(std::move(sceneName))
    , m_pRootObject(new GameObject(*this, 0, 0, "Scene root"))
{
}

void Scene::Load()
{
    m_active = true;

    if (m_initFunction and not m_initialised)
    {
        (*m_initFunction)(*this);
        m_initialised = true;
    }
    m_pRootObject->Activate();
}

void Scene::Unload()
{
    m_pRootObject->Deactivate();

    m_active = false;
}

void Scene::Reset()
{
    if (not m_initialised)
        return;

    if (m_unloadFunction)
        (*m_unloadFunction)();

    m_pRootObject.reset(new GameObject(*this, 0, 0, "Scene root"));
    m_initialised = false;
}

void Scene::RemoveGameObject(GameObject* pObject) const
{
    assert(pObject != nullptr && "pObject is nullptr");
    m_pRootObject->RemoveChild(pObject);
}

auto Scene::IsActive() const -> bool
{
    return m_active;
}

void Scene::QueueReparent(GameObject& child, GameObject& newParent, bool keepWorldPosition)
{
    m_reparentQueue.emplace(child, newParent, keepWorldPosition);
}

void Scene::ExecuteReparentingQueue()
{
    while (not m_reparentQueue.empty())
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
        [](Renderable const* pComp1, Renderable const* pComp2) -> bool { return pComp1->GetZIndex() < pComp2->GetZIndex(); });
}

auto Scene::GetRoot() const -> GameObject*
{
    return m_pRootObject.get();
}

void Scene::Update() const
{
    m_pRootObject->Update();
}

void Scene::FixedUpdate() const
{
    m_pRootObject->FixedUpdate();
}

void Scene::LateFixedUpdate() const
{
    m_pRootObject->LateFixedUpdate();
}

void Scene::LateUpdate() const
{
    m_pRootObject->LateUpdate();
}

void Scene::Render() const
{
    for (auto* renderComponents : m_renderComponents)
    {
        if (renderComponents->m_pOwner->IsActive())
            renderComponents->Render();
    }
}

void Scene::DrawUI() const
{
    for (const auto* uiComponent : m_uiComponents)
    {
        if (uiComponent->m_pOwner->IsActive())
            uiComponent->DrawUI();
    }
}

auto Scene::operator==(const Scene& other) const -> bool
{
    return other.m_pRootObject.get() == m_pRootObject.get();
}


}  // namespace gla