#include "Components/Sprite.hpp"

#include <stdexcept>
#include <utility>

#include "GameObject.hpp"
#include "Renderer.hpp"
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

Sprite::~Sprite() noexcept
{
    auto& sm = SceneManager::Get();
    sm.UnregisterRenderComponent(this);
}

void Sprite::Update(float /*deltaTime*/) {}

void Sprite::Render()
{
    if(not m_texture or not m_Visible)
        return;

    const auto& pos{ m_pOwner->GetWorldPosition() };
    const auto& scale{ m_pOwner->GetTransform().GetWorldScale() };

    Renderer::Get().RenderTextureScale(*m_texture, pos.x, pos.y, scale.x, scale.y);
}

void Sprite::SetTexture(std::shared_ptr<Texture2D> texture)
{
    m_texture = std::move(texture);
}

void Sprite::SetSourceRect(SDL_FRect sourceRect)
{
    if(sourceRect.x < 0 or sourceRect.y < 0 or sourceRect.w < 0 or sourceRect.h < 0)
        throw std::invalid_argument("arguments must be greater than 0");

    m_sourceRect = sourceRect;
}

void Sprite::SetSourceRect(float x, float y, float w, float h)
{
    if(x < 0 or y < 0 or w < 0 or h < 0)
        throw std::invalid_argument("arguments must be greater than 0");

    m_sourceRect.x = x;
    m_sourceRect.y = y;
    m_sourceRect.w = w;
    m_sourceRect.h = h;
}

void Sprite::SetSourceRectPos(float x, float y)
{
    if(x < 0 or y < 0)
        throw std::invalid_argument("arguments must be greater than 0");

    m_sourceRect.x = x;
    m_sourceRect.y = y;
}

void Sprite::SetSourceRectSize(float w, float h)
{
    if(w < 0 or h < 0)
        throw std::invalid_argument("arguments must be greater than 0");

    m_sourceRect.w = w;
    m_sourceRect.h = h;
}

SDL_FRect Sprite::GetSourceRect() const
{
    return m_sourceRect;
}

}  // namespace gla
