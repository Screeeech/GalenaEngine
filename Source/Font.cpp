#include "Font.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <print>
#include <stdexcept>

namespace gla
{

Font::Font(std::string const& fullPath, float size)
    : m_font(TTF_OpenFont(fullPath.c_str(), size))
{
    if(m_font == nullptr)
    {
        throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
    }
}

Font::~Font() noexcept
{
    TTF_CloseFont(m_font);
}

auto Font::GetFont() const -> TTF_Font*
{
    return m_font;
}

}  // namespace gla