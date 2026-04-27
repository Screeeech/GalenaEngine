#ifndef GALENA_GAMEOBJECT_HPP
#define GALENA_GAMEOBJECT_HPP

#include <glm/fwd.hpp>
#include <memory>
#include <ranges>
#include <vector>

#include "Component.hpp"
#include "Transform.hpp"

namespace gla
{

class Transform;
class Sprite;
class Texture2D;

class GameObject final
{
public:
    void Update(float deltaTime) const;

    explicit GameObject(float x, float y, float z = 0.0f, std::string_view name = "new GameObject");
    ~GameObject() noexcept = default;

    GameObject(GameObject const& other) = delete;
    auto operator=(GameObject const& other) -> GameObject& = delete;
    GameObject(GameObject&& other) = delete;
    auto operator=(GameObject&& other) -> GameObject& = delete;

    template<ComponentConcept T, typename... Args>
    auto AddComponent(Args&&... args) noexcept -> T*
    {
        m_components.push_back(std::make_unique<T>(this, std::forward<Args>(args)...));
        return dynamic_cast<T*>(m_components.back().get());
    }

    template<ComponentConcept T>
    void RemoveComponent() noexcept
    {
        for(auto& component : m_components)
            if(auto* pComponent = dynamic_cast<T*>(component.get()))
                m_components.erase(pComponent);
    }

    template<ComponentConcept T>
    auto GetComponent() -> T*
    {
        for(auto& component : m_components)
            if(auto* pComponent = dynamic_cast<T*>(component.get()))
                return pComponent;
        return nullptr;
    }

    template<ComponentConcept T>
    auto GetAllComponents()
    {
        return m_components | std::views::transform([](auto const& comp) -> auto { return dynamic_cast<T*>(comp.get()); }) |
            std::views::filter([](auto* comp) { return comp != nullptr; });
    }

    auto GetTransform() -> Transform&;
    auto GetParentWorldMatrix() -> glm::mat4;
    auto GetWorldPosition() -> glm::vec3;

    auto CreateChild(float x, float y, float z = 0.0f, std::string_view name = "new GameObject") -> GameObject*;
    auto DisownChild(GameObject* pChild) -> std::unique_ptr<GameObject>;
    auto RemoveChild(GameObject* pChild) -> bool;
    void Reparent(GameObject* pParent, bool keepWorldPosition = true);

    // Recursive function
    void SetDirty();

    // For testing purposes
    std::string_view m_name;

private:
    void AddChild(std::unique_ptr<GameObject> pChild);
    auto IsChild(GameObject* pChild) -> bool;


    GameObject* m_pParent{};
    // NOTE: Should the parent own the children?
    std::vector<std::unique_ptr<GameObject>> m_children;

    std::vector<std::unique_ptr<Component>> m_components;
    Transform m_transform;
};

template<typename ObjectType>
concept GameObjectConcept = std::derived_from<ObjectType, GameObject>;

}  // namespace gla

#endif  // GALENA_GAMEOBJECT_HPP