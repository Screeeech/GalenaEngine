#include "GameObject.hpp"

#include <algorithm>
#include <print>
#include <ranges>
#include <utility>

#include "Scene.hpp"
#include "Services/ResourceManager.hpp"

namespace vw = std::ranges::views;
namespace rng = std::ranges;

namespace gla
{

void GameObject::Update() const
{
    for (const auto& component : m_components)
        if (component->m_active)
            component->Update();

    for (const auto& child : m_children)
        if (child->IsActive())
            child->Update();
}

void GameObject::FixedUpdate() const
{
    for (const auto& component : m_components)
        if (component->m_active)
            component->FixedUpdate();

    for (const auto& child : m_children)
        if (child->IsActive())
            child->FixedUpdate();
}

void GameObject::LateUpdate() const
{
    for (const auto& component : m_components)
        if (component->m_active)
            component->LateUpdate();

    for (const auto& child : m_children)
        if (child->IsActive())
            child->LateUpdate();
}

void GameObject::LateFixedUpdate() const
{
    for (const auto& component : m_components)
        if (component->m_active)
            component->LateFixedUpdate();

    for (const auto& child : m_children)
        if (child->IsActive())
            child->LateFixedUpdate();
}

GameObject::~GameObject() noexcept
{
    Deactivate();
}

void GameObject::Activate()
{
    if (not m_active)
    {
        for (auto const& component : m_components)
            component->Activate();
        for (auto const& child : m_children)
            child->Activate();
        m_active = true;
    }
}

void GameObject::Deactivate()
{
    if (m_active)
    {
        for (auto const& component : m_components)
            component->Deactivate();
        for (auto const& child : m_children)
            child->Deactivate();

        m_active = false;
    }
}

auto GameObject::IsActive() const -> bool
{
    return m_active;
}

auto GameObject::RemoveComponent(Component* pComponent) -> bool
{
    auto const it = rng::find_if(m_components, [&](auto const& comp) -> bool { return pComponent == comp.get(); });
    if (it != m_components.end())
    {
        auto [fst, snd] = m_componentMap.equal_range(std::type_index(typeid(pComponent)));
        for (auto mapIt = fst; mapIt != snd; ++mapIt)
        {
            if (mapIt->second == pComponent)
            {
                m_componentMap.erase(mapIt);
                break;
            }
        }

        pComponent->Deactivate();
        m_components.erase(it);
        return true;
    }
    return false;
}

auto GameObject::GetFirstChild() -> GameObject*
{
    if (auto const children = GetChildren(); children.begin() != children.end())
        return *children.begin();

    return nullptr;
}

auto GameObject::GetFirstChild(std::string_view tag) -> GameObject*
{
    if (auto children = GetChildren(tag); children.begin() != children.end())
        return *children.begin();

    return nullptr;
}

auto GameObject::GetTransform() -> Transform&
{
    return m_transform;
}

auto GameObject::GetParentWorldPosition() const -> glm::vec2
{
    if (not m_pParent)
        return glm::vec2{ 0.0f };

    return m_pParent->m_transform.GetWorldPosition();
}

auto GameObject::GetWorldPosition() -> glm::vec2
{
    return GetTransform().GetWorldPosition();
}

auto GameObject::GetLocalPosition() -> glm::vec2
{
    return GetTransform().GetLocalPosition();
}

auto GameObject::CreateChild(glm::vec2 startPosition, std::string const& name) -> GameObject*
{
    return CreateChild(startPosition.x, startPosition.y, name);
}

auto GameObject::CreateChild(float x, float y, std::string const& name) -> GameObject*
{
    m_children.emplace_back(new GameObject(*m_parentScene, x, y, name));

    GameObject* child = m_children.back().get();
    child->m_pParent = this;

    return child;
}

auto GameObject::DisownChild(GameObject* pChild) -> std::unique_ptr<GameObject>
{
    // We have to move ownership of the child pointer from the parent
    // and return it to instantiate a new unique_ptr
    // I think this is a good approach?
    auto const it = rng::find_if(m_children, [pChild](const auto& child) { return child.get() == pChild; });

    std::unique_ptr<GameObject> newChild{};
    if (it != m_children.end())
    {
        // I'm not sure if this is a good idea, but we ball lol
        // Releasing here "deactivates" the unique_ptr so erasing it doesn't free the GameObject
        newChild = std::move(*it);

        // Thus we can safely remove it from the list while keeping the pointer and returning it
        m_children.erase(it);
    }

    return newChild;
}

bool GameObject::RemoveChild(GameObject* pChild)
{
    // See if child is in current GameObject's list of children
    auto const it = rng::find_if(m_children, [pChild](const auto& child) { return child.get() == pChild; });
    if (it != m_children.end())
    {
        m_children.erase(it);
        return true;
    }

    // Otherwise check children of children
    for (const auto& child : m_children)
    {
        if (child->RemoveChild(pChild))
            return true;
    }

    return false;
}
void GameObject::QueueReparent(GameObject& newParent, bool keepWorldPosition)
{
    if (m_pParent == nullptr)
    {
        std::println("Warning! cannot reparent the scene root object");
        return;
    }
    if (IsChild(newParent))
    {
        std::println("Warning! GameObject is already a child of the given parent");
        return;
    }
    if (&newParent == this)
    {
        std::println("Warning! Cannot reparent GameObject to itself");
        return;
    }
    if (&newParent == m_pParent)
    {
        std::println("Warning! GameObject is already parented to this parent");
        return;
    }
    // if (m_parentScene != newParent.m_parentScene)
    //{
    //     std::println("Warning! Cannot reparent GameObject to a GameObject of a different scene");
    //     return;
    // }

    m_parentScene->QueueReparent(*this, newParent, keepWorldPosition);
}

auto GameObject::GetParentScene() const -> Scene&
{
    return *m_parentScene;
}

auto GameObject::GetParent() const -> GameObject*
{
    return m_pParent;
}

void GameObject::QueueDelete()
{
    if (m_pParent == nullptr)
    {
        std::println("Warning!\tYou cannot delete the scene root object");
        return;
    }

    m_shouldDelete = true;
}

void GameObject::Reparent(GameObject& newParent, bool keepWorldPosition)
{
    // Is parent root?
    if (not newParent.m_pParent)  // Then set local pos to world pos
    {
        m_transform.SetLocalPosition(m_transform.GetWorldPosition());
    }
    else  // If it isn't root, then check if we need to keep the world position
    {
        if (keepWorldPosition)
            m_transform.SetLocalPosition(GetWorldPosition() - newParent.GetWorldPosition());
        else
            SetDirty();
    }

    // Remove the object itself from the current parent's list of children
    std::unique_ptr<GameObject> self = m_pParent->DisownChild(this);

    // Set new parent
    m_pParent = &newParent;
    m_parentScene = &newParent.GetParentScene();

    // Add this as a new child to parent's list of children
    m_pParent->AddChild(std::move(self));
}

void GameObject::SetDirty()
{
    if (m_transform.isDirty)
        return;

    m_transform.isDirty = true;
    for (const auto& child : m_children)
        child->SetDirty();
}

GameObject::GameObject(Scene& parentScene, float x, float y, std::string_view name)
    : m_name(name)
    , m_parentScene(&parentScene)
    , m_transform(x, y, this)
{
}

void GameObject::AddChild(std::unique_ptr<GameObject> pChild)
{
    if (not pChild or pChild.get() == this or IsChild(*pChild))
        return;

    m_children.push_back(std::move(pChild));
}

auto GameObject::IsChild(GameObject& pChild) -> bool
{
    const auto it = rng::find(m_children, &pChild, [](auto const& child) { return child.get(); });
    return it != m_children.end();
}

void GameObject::DeleteMarkedObjects()
{
    std::erase_if(m_children, [](std::unique_ptr<GameObject> const& child) -> bool { return child->m_shouldDelete; });

    for (auto const& child : m_children)
    {
        child->DeleteMarkedObjects();
    }
}

}  // namespace gla
