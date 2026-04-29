#ifndef GALENA_RESOURCEMANAGER_HPP
#define GALENA_RESOURCEMANAGER_HPP

#include <SDL3/SDL_surface.h>

#include <filesystem>
#include <map>
#include <memory>
#include <string>

namespace gla
{
class Texture2D;
class Font;

// TODO: Rework this entire class cause this is pretttyyy bad
class ResourceManager final
{
public:
    ResourceManager();
    ~ResourceManager() = default;

    ResourceManager(ResourceManager const&) = delete;
    auto operator=(ResourceManager const&) -> ResourceManager& = delete;
    ResourceManager(ResourceManager&&) = delete;
    auto operator=(ResourceManager&&) -> ResourceManager& = delete;


    auto LoadTexture(std::string const& file, SDL_ScaleMode scaleMode = SDL_SCALEMODE_PIXELART) -> std::shared_ptr<Texture2D>;
    auto LoadFont(std::string const& filePath, uint8_t size) -> std::shared_ptr<Font>;

private:

    void UnloadUnusedResources();

    std::map<std::string, std::shared_ptr<Texture2D>> m_loadedTextures;
    std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_loadedFonts;
};

}  // namespace gla

#endif // GALENA_RESOURCEMANAGER_HPP