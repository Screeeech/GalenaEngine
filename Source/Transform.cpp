#include "Transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "GameObject.hpp"

namespace gla
{

Transform::Transform(float x, float y, GameObject* parent)
    : m_pOwner(parent)
    , m_localPosition(x, y)
    , m_worldPosition(x, y)
{
}

Transform::Transform(const Transform& transform, GameObject* owner)
    : m_pOwner(owner)
    , m_localPosition(transform.m_localPosition)
{
}

void Transform::SetLocalPosition(const float x, const float y)
{
    m_localPosition.x = x;
    m_localPosition.y = y;

    m_pOwner->SetDirty();
}

void Transform::SetLocalPosition(const glm::vec2& position)
{
    m_localPosition = position;

    m_pOwner->SetDirty();
}

void Transform::ChangeLocalPosition(float x, float y)
{
    m_localPosition.x += x;
    m_localPosition.y += y;

    m_pOwner->SetDirty();
}

void Transform::ChangeLocalPosition(const glm::vec2& delta)
{
    m_localPosition += delta;

    m_pOwner->SetDirty();
}

auto Transform::GetLocalPosition() const -> glm::vec2
{
    // Doesn't have to set dirty flag, because it doesn't affect children
    return m_localPosition;
}

auto Transform::GetWorldPosition() -> glm::vec2
{
    if (isDirty)
        UpdateWorldPosition();

    return m_worldPosition;
}

void Transform::UpdateWorldPosition()
{
    m_worldPosition = m_pOwner->GetParentWorldPosition() + m_localPosition;
    isDirty = false;
}

void Transform::ApplyInverseTransform(const Transform& parentTransform)
{
    m_worldPosition -= parentTransform.m_worldPosition;
}

}  // namespace gla