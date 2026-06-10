#ifndef GALENA_GAMEOBJECT_HPP
#define GALENA_GAMEOBJECT_HPP

#include <glm/fwd.hpp>
#include <memory>
#include <print>
#include <ranges>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Component.hpp"
#include "Scene.hpp"
#include "Transform.hpp"


namespace gla
{
class Scene;

class Transform;
class Sprite;
class Texture2D;

class GameObject final
{
public:
    void Update() const;
    void FixedUpdate() const;
    void LateUpdate() const;
    void LateFixedUpdate() const;

    ~GameObject() noexcept;

    void Activate();
    void Deactivate();
    auto IsActive() const -> bool;

    template<ComponentConcept T, typename... Args>
    auto AddComponent(Args&&... args) noexcept -> T*
    {
        m_components.emplace_back(std::make_unique<T>(this, std::forward<Args>(args)...));
        auto* component = static_cast<T*>(m_components.back().get());
        m_componentMap.insert({ std::type_index(typeid(T)), component });

        // If the current scene is already active, we should activate the new component immediately
        if (m_parentScene.IsActive() and IsActive())
            component->Activate();

        return component;
    }

    template<ComponentConcept T>
    auto RemoveComponent() noexcept -> bool
    {
        auto const count = m_componentMap.count(typeid(T));
        if (count > 1)
        {
            std::println(
                "Warning!\tTrying to call RemoveComponent<{}>() when there are multiple components stored of the same type can lead to any of "
                "the components being removed",
                typeid(T).name());
            std::println("\tInstead, use RemoveComponent(Component* pComponent) to remove a specific component");
            return false;
        }
        if (count == 0)
            return false;

        auto const it = m_componentMap.find(typeid(T));
        it->second->Deactivate();
        m_componentMap.erase(it);
        std::erase_if(m_components, [&](auto const& comp) -> bool { return it->second == comp.get(); });

        return true;
    }
    auto RemoveComponent(Component* pComponent) -> bool;


    template<ComponentConcept T>
    auto GetComponent() const -> T*
    {
        // If there are multiple components of the same type, any of them can be return here
        // Use GetComponents() instead to get all of the components
        if (auto const it = m_componentMap.find(std::type_index(typeid(T))); it != m_componentMap.end())
            return static_cast<T*>(it->second);
        return nullptr;
    }

    template<ComponentConcept T>
    auto GetComponents() const
    {
        // clang-format off
        auto [fst, snd] = m_componentMap.equal_range(std::type_index(typeid(T)));
        return std::ranges::subrange(fst, snd)
             | std::views::transform(
                 [](auto const& comp) -> T* { return static_cast<T*>(comp.get()); });
        // clang-format on
    }

    [[nodiscard]] auto GetChildren() const
    {
        return m_children | std::views::transform([](std::unique_ptr<GameObject> const& child) -> GameObject const* { return child.get(); });
    }

    auto GetTransform() -> Transform&;
    auto GetParentWorldPosition() const -> glm::vec2;
    auto GetWorldPosition() -> glm::vec2;
    auto GetLocalPosition() -> glm::vec2;

    auto CreateChild(glm::vec2 startPosition, std::string const& name = "new GameObject") -> GameObject*;
    auto CreateChild(float x, float y, std::string const& name = "new GameObject") -> GameObject*;
    auto DisownChild(GameObject* pChild) -> std::unique_ptr<GameObject>;
    auto RemoveChild(GameObject* pChild) -> bool;
    void QueueReparent(GameObject& newParent, bool keepWorldPosition = true);

    auto GetParentScene() const -> Scene&;

    // Recursive function
    void SetDirty();

    // For testing purposes
    std::string m_name;


    GameObject(GameObject const& other) = delete;
    auto operator=(GameObject const& other) -> GameObject& = delete;
    GameObject(GameObject&& other) = delete;
    auto operator=(GameObject&& other) -> GameObject& = delete;

private:
    // I only want to be able to GameObjects from my scene or from other game objects
    // So the constructor is private
    friend class Scene;
    explicit GameObject(Scene& parentScene, float x, float y, std::string_view name = "new GameObject");
    void Reparent(GameObject& newParent, bool keepWorldPosition = true);

    void AddChild(std::unique_ptr<GameObject> pChild);
    auto IsChild(GameObject& pChild) -> bool;

    bool m_active{ false };

    Scene& m_parentScene;
    GameObject* m_pParent{};
    std::vector<std::unique_ptr<GameObject>> m_children;

    std::vector<std::unique_ptr<Component>> m_components;
    std::unordered_multimap<std::type_index, Component*> m_componentMap;
    Transform m_transform;

    // TODO:
    // Create a component handler for my transforms, this will hold an ID for the component and a cached pointer
    // I will try to overload the derefence operators to be able to easily use it and use the dirty flag pattern to try
    // and figure out when I need to reacquire the pointer to the component
    // this way I will be able to safely hold the reference without it becoming dangling at any point
};

template<typename ObjectType>
concept GameObjectConcept = std::derived_from<ObjectType, GameObject>;

}  // namespace gla

#endif  // GALENA_GAMEOBJECT_HPP