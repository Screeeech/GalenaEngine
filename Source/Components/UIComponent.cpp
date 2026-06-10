#include "Components/UIComponent.hpp"

#include <utility>

#include "GameObject.hpp"
#include "Locator.hpp"
#include "Services/Renderer.hpp"
#include "Services/SceneManager.hpp"

namespace gla
{
class Renderer;

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
    Locator::Get<Renderer>().RegisterUIComponent(this);
}

void UIComponent::OnDeactivate()
{
    Locator::Get<Renderer>().UnregisterUIComponent(this);
}

}  // namespace gla
