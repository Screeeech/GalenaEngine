#include "ResourceManager.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <stdexcept>

#include "Font.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"

namespace fs = std::filesystem;

namespace gla
{

void ResourceManager::Init(const std::filesystem::path& dataPath)
{
    m_dataPath = dataPath;

    if (!TTF_Init())
    {
        throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
    }
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& file, SDL_ScaleMode scaleMode)
{
    // const auto fullPath = m_dataPath/file;
    const auto filename = fs::path(file).filename().string();
    if(not m_loadedTextures.contains(filename))
        m_loadedTextures.insert(std::pair(filename,std::make_shared<Texture2D>(file, scaleMode)));
    return m_loadedTextures.at(filename);
}

std::shared_ptr<Font> ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
    // const auto fullPath = m_dataPath/file;
    const auto filename = fs::path(file).filename().string();
    const auto key = std::pair(filename, size);
    if(not m_loadedFonts.contains(key))
        m_loadedFonts.insert(std::pair(key,std::make_shared<Font>(filename, size)));
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