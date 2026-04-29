#include "Services/ResourceManager.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <stdexcept>

#include "Services/Renderer.hpp"
#include "Font.hpp"
#include "Texture2D.hpp"

namespace gla
{

ResourceManager::ResourceManager()
{
    if (!TTF_Init())
        throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
}

auto ResourceManager::LoadTexture(std::string const& filePath, SDL_ScaleMode scaleMode) -> std::shared_ptr<Texture2D>
{
    if(not m_loadedTextures.contains(filePath))
        m_loadedTextures.insert(std::pair(filePath,std::make_shared<Texture2D>(filePath, scaleMode)));
    return m_loadedTextures.at(filePath);
}

auto ResourceManager::LoadFont(std::string const& filePath, uint8_t size) -> std::shared_ptr<Font>
{
    // const auto fullPath = m_dataPath/file;
    const auto key = std::pair(filePath, size);
    if(not m_loadedFonts.contains(key))
        m_loadedFonts.insert(std::pair(key,std::make_shared<Font>(filePath, size)));
    return m_loadedFonts.at(key);
}

void ResourceManager::UnloadUnusedResources()
{
    for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end();)
    {
        if (it->second.use_count() == 1)
            it = m_loadedTextures.erase(it);
        else
            ++it;
    }

    for (auto it = m_loadedFonts.begin(); it != m_loadedFonts.end();)
    {
        if (it->second.use_count() == 1)
            it = m_loadedFonts.erase(it);
        else
            ++it;
    }
}

}