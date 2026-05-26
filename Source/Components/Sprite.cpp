#include "Components/Sprite.hpp"

#include <stdexcept>
#include <utility>

#include "GameObject.hpp"
#include "Locator.hpp"
#include "Services/Renderer.hpp"
#include "Texture2D.hpp"
#include "Transform.hpp"

namespace gla
{

Sprite::Sprite(GameObject* pOwner, std::shared_ptr<Texture2D> texture, int zIndex)
    : Renderable(pOwner, zIndex)
    , m_texture(std::move(texture))
    , m_sourceRect(0, 0, m_texture->GetSize().x, m_texture->GetSize().y)
{
}

Sprite::Sprite(GameObject* pOwner, int zIndex)
    : Renderable(pOwner, zIndex)
    , m_sourceRect()
{
}

void Sprite::Render()
{
    if (not m_texture or not m_Visible)
        return;

    const auto& pos{ m_pOwner->GetWorldPosition() };

    Locator::Get<Renderer>().RenderTexture(*m_texture, pos.x, pos.y);
}

void Sprite::SetTexture(std::shared_ptr<Texture2D> texture)
{
    m_texture = std::move(texture);
}

void Sprite::SetSourceRect(SDL_FRect sourceRect)
{
    if (sourceRect.x < 0 or sourceRect.y < 0 or sourceRect.w < 0 or sourceRect.h < 0)
        throw std::invalid_argument("arguments must be greater than 0");

    m_sourceRect = sourceRect;
}

void Sprite::SetSourceRect(float x, float y, float w, float h)
{
    if (x < 0 or y < 0 or w < 0 or h < 0)
        throw std::invalid_argument("arguments must be greater than 0");

    m_sourceRect.x = x;
    m_sourceRect.y = y;
    m_sourceRect.w = w;
    m_sourceRect.h = h;
}

void Sprite::SetSourceRectPos(float x, float y)
{
    if (x < 0 or y < 0)
        throw std::invalid_argument("arguments must be greater than 0");

    m_sourceRect.x = x;
    m_sourceRect.y = y;
}

void Sprite::SetSourceRectSize(float w, float h)
{
    if (w < 0 or h < 0)
        throw std::invalid_argument("arguments must be greater than 0");

    m_sourceRect.w = w;
    m_sourceRect.h = h;
}

auto Sprite::GetSourceRect() const -> SDL_FRect
{
    return m_sourceRect;
}

}  // namespace gla
