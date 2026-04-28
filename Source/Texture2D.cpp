#include "Texture2D.hpp"

#include <SDL3/SDL.h>

#include <stdexcept>

#include "ServiceLocator.hpp"
#include "Services/Renderer.hpp"

namespace gla
{

Texture2D::~Texture2D() noexcept
{
    SDL_DestroyTexture(m_texture);
}

glm::vec2 Texture2D::GetSize() const
{
    float w{};
    float h{};
    SDL_GetTextureSize(m_texture, &w, &h);
    return { w, h };
}

SDL_Texture* Texture2D::GetSDLTexture() const
{
    return m_texture;
}

Texture2D::Texture2D(const std::string& fullPath, SDL_ScaleMode scaleMode)
{
    auto const* renderer = ServiceLocator::Request<Renderer>().value_or(nullptr);
    if (not renderer)
    {
        std::println("Careful! No renderer was found, proceeding without rendering texture");
        return;
    }


    SDL_Surface* surface = SDL_LoadPNG(fullPath.c_str());
    if(!surface)
    {
        throw std::runtime_error(std::string("Failed to load PNG: ") + SDL_GetError());
    }

    m_texture = SDL_CreateTextureFromSurface(renderer->GetSDLRenderer(), surface);
    SDL_SetTextureScaleMode(m_texture, scaleMode);

    SDL_DestroySurface(surface);

    if(!m_texture)
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

}