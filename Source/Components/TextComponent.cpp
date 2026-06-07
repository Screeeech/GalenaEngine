#include "Components/TextComponent.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <stdexcept>
#include <utility>

#include "Components/Sprite.hpp"
#include "GameObject.hpp"
#include "Locator.hpp"
#include "Services/Renderer.hpp"
#include "Texture2D.hpp"

namespace gla
{


TextComponent::TextComponent(GameObject* pOwner, std::string text, std::shared_ptr<Font> font, int zIndex, Align alignment, SDL_Color color)
    : Component(pOwner)
    , m_Font(std::move(font))
    , m_alignment(alignment)
    , m_Color(color)
    , m_Text(std::move(text))
    , m_zIndex(zIndex)
    , m_pTextSprite(m_pOwner->AddComponent<Sprite>(m_zIndex))
{
}

void TextComponent::SetText(std::string const& text)
{
    m_Text = text;
    m_NeedsUpdate = true;
}
auto TextComponent::GetText() const -> std::string const&
{
    return m_Text;
}

void TextComponent::OnActivate()
{
    m_TextTexture = UpdateTexture();
    m_pTextSprite->SetTexture(m_TextTexture);
}

void TextComponent::Update()
{
    if (m_Text.empty() or not m_NeedsUpdate)
        return;

    m_NeedsUpdate = false;
    m_TextTexture = UpdateTexture();
    m_pTextSprite->SetTexture(m_TextTexture);
}

auto TextComponent::UpdateTexture() const -> std::shared_ptr<Texture2D>
{
    auto* const surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Text.length(), m_Color);
    if (surf == nullptr)
    {
        throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
    }
    auto* texture = SDL_CreateTextureFromSurface(Locator::Get<Renderer>().GetSDLRenderer(), surf);
    if (texture == nullptr)
    {
        throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
    }
    SDL_DestroySurface(surf);

    if (m_alignment == Align::Left)
    {
        m_pTextSprite->m_offset = {};
    }
    else if (m_alignment == Align::Right)
    {
        m_pTextSprite->m_offset = { -texture->w, 0 };
    }

    return std::make_shared<Texture2D>(texture);
}

}  // namespace gla
