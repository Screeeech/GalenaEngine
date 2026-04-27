#include "Font.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <stdexcept>

namespace gla
{

Font::Font(const std::string& fullPath, float size)
    : m_font(nullptr)
{
    m_font = TTF_OpenFont(fullPath.c_str(), size);
    if(m_font == nullptr)
    {
        throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
    }
}

Font::~Font() noexcept
{
    TTF_CloseFont(m_font);
}

TTF_Font* Font::GetFont() const
{
    return m_font;
}

}  // namespace gla