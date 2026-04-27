#ifndef GALENA_TRANSFORM_HPP
#define GALENA_TRANSFORM_HPP

#include <glm/glm.hpp>

namespace gla
{

class GameObject;

class Transform final
{
public:
    explicit Transform(float x, float y, float z = 0, GameObject* parent = nullptr);
    explicit Transform(const Transform& transform, GameObject* owner);

    // Position
    void SetLocalPosition(float x, float y, float z = 0);
    void SetLocalPosition(const glm::vec3& position);
    void ChangeLocalPosition(float x, float y, float z = 0);
    void ChangeLocalPosition(const glm::vec3& delta);
    [[nodiscard]] auto GetLocalPosition() const -> glm::vec3;
    [[nodiscard]] auto GetWorldPosition() -> glm::vec3;

    // Rotation
    // TODO: Add once I need it :P

    // Scale
    void SetScale(float x, float y, float z = 1);
    [[nodiscard]] auto GetLocalScale() const -> glm::vec2;
    [[nodiscard]] auto GetWorldScale() -> glm::vec2;


    // World
    [[nodiscard]] auto GetWorldMatrix() -> glm::mat4;
    void UpdateWorldMatrix();
    void ApplyInverseTransform(const Transform& parentTransform);

    bool isDirty{true};

private:
    GameObject* m_pOwner{};

    glm::mat4 m_worldMatrix{1.f};

    glm::vec3 m_localRotation{0.f};
    glm::vec3 m_localScale{1.f};
    glm::vec3 m_localPosition{0.f};

    glm::vec3 m_worldRotation{0.f};
    glm::vec3 m_worldScale{1.f};
    glm::vec3 m_worldPosition{0.f};
};

}  // namespace gla

#endif  // GALENA_TRANSFORM_HPP