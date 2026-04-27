#include "Components/UIComponent.hpp"

#include <utility>

#include "SceneManager.hpp"

namespace gla
{

UIComponent::UIComponent(GameObject* pOwner, std::function<void(GameObject* pCaller)> drawFunc)
    : Component(pOwner)
    , m_drawFunc(std::move(drawFunc))
{
    SceneManager::Get().RegisterUIComponent(this);
}

UIComponent::~UIComponent() noexcept
{
    SceneManager::Get().UnregisterUIComponent(this);
}

void UIComponent::Update(float /*deltaTime*/) {}

void UIComponent::DrawUI() const
{
    m_drawFunc(m_pOwner);
}

}  // namespace gla
