#include "Texture2D.hpp"

#include <SDL3/SDL.h>

#include <stdexcept>

#include "Locator.hpp"
#include "Services/Renderer.hpp"

namespace gla
{

Texture2D::~Texture2D() noexcept
{
    SDL_DestroyTexture(m_texture);
}

auto Texture2D::GetSize() const -> glm::vec2
{
    float w{};
    float h{};
    SDL_GetTextureSize(m_texture, &w, &h);
    return { w, h };
}

auto Texture2D::GetSDLTexture() const -> SDL_Texture*
{
    return m_texture;
}

Texture2D::Texture2D(const std::string& fullPath, SDL_ScaleMode scaleMode)
{
    SDL_Surface* surface = SDL_LoadPNG(fullPath.c_str());
    if (not surface)
    {
        throw std::runtime_error(std::string("Failed to load PNG: ") + SDL_GetError());
    }

    m_texture = SDL_CreateTextureFromSurface(Locator::Get<Renderer>().GetSDLRenderer(), surface);
    SDL_SetTextureScaleMode(m_texture, scaleMode);

    SDL_DestroySurface(surface);

    if (not m_texture)
    {
        throw std::runtime_error(std::string("Failed to create texture from surface: ") + SDL_GetError());
    }
}

Texture2D::Texture2D(SDL_Texture* texture, SDL_ScaleMode scaleMode)
    : m_texture{ texture }
{
    assert(m_texture != nullptr);
    SDL_SetTextureScaleMode(m_texture, scaleMode);
}

}  // namespace gla