#ifndef GALENA_RESOURCEMANAGER_HPP
#define GALENA_RESOURCEMANAGER_HPP

#include <SDL3/SDL_surface.h>

#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "Singleton.hpp"

namespace gla
{
class Texture2D;
class Font;

class ResourceManager final : public Singleton<ResourceManager>
{
public:
    static void Init();
    auto LoadTexture(std::string const& file, SDL_ScaleMode scaleMode = SDL_SCALEMODE_LINEAR) -> std::shared_ptr<Texture2D>;
    auto LoadFont(std::string const& filePath, uint8_t size) -> std::shared_ptr<Font>;

private:
    friend class Singleton<ResourceManager>;
    ResourceManager() = default;

    void UnloadUnusedResources();

    std::map<std::string, std::shared_ptr<Texture2D>> m_loadedTextures;
    std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_loadedFonts;
};

}  // namespace gla

#endif // GALENA_RESOURCEMANAGER_HPP