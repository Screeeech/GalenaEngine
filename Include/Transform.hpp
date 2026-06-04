#ifndef GALENA_TRANSFORM_HPP
#define GALENA_TRANSFORM_HPP

#include <glm/glm.hpp>

namespace gla
{

class GameObject;

class Transform final
{
public:
    explicit Transform(float x, float y, GameObject* parent = nullptr);
    explicit Transform(const Transform& transform, GameObject* owner);

    void SetLocalPosition(float x, float y);
    void SetLocalPosition(const glm::vec2& position);
    void ChangeLocalPosition(float x, float y);
    void ChangeLocalPosition(const glm::vec2& delta);
    [[nodiscard]] auto GetLocalPosition() const -> glm::vec2;
    [[nodiscard]] auto GetWorldPosition() -> glm::vec2;

    void UpdateWorldPosition();

    bool isDirty{ true };

private:
    GameObject* m_pOwner{};

    glm::vec2 m_localPosition{ 0.f };
    glm::vec2 m_worldPosition{ 0.f };
};

}  // namespace gla

#endif  // GALENA_TRANSFORM_HPP