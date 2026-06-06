#include "Components/UIComponent.hpp"

#include <utility>

#include "Services/SceneManager.hpp"
#include "GameObject.hpp"
#include "Locator.hpp"

namespace gla
{

UIComponent::UIComponent(GameObject* pOwner, std::function<void(GameObject* pCaller)> drawFunc)
    : Component(pOwner)
    , m_drawFunc(std::move(drawFunc))
{
}

void UIComponent::DrawUI() const
{
    m_drawFunc(m_pOwner);
}

void UIComponent::OnActivate()
{
    m_pOwner->GetParentScene().RegisterUIComponent(this);
}

void UIComponent::OnDeactivate()
{
    m_pOwner->GetParentScene().UnregisterUIComponent(this);
}

}  // namespace gla
