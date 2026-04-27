#ifndef GALENA_TEXTURE2D_HPP
#define GALENA_TEXTURE2D_HPP

#include <SDL3/SDL_surface.h>

#include <glm/vec2.hpp>
#include <string>

struct SDL_Texture;

namespace gla
{

class Texture2D final
{
public:
    [[nodiscard]] auto GetSDLTexture() const -> SDL_Texture*;
    explicit Texture2D(SDL_Texture* texture, SDL_ScaleMode scaleMode = SDL_SCALEMODE_LINEAR);

    explicit Texture2D(const std::string& fullPath, SDL_ScaleMode scaleMode = SDL_SCALEMODE_LINEAR);

    ~Texture2D() noexcept;

    [[nodiscard]] auto GetSize() const -> glm::vec2;

    Texture2D(Texture2D const&) = delete;
    auto operator=(Texture2D const&) -> Texture2D& = delete;
    Texture2D(Texture2D&&) = delete;
    auto operator=(const Texture2D&&) -> Texture2D& = delete;

private:
    SDL_Texture* m_texture;
};

}  // namespace gla

#endif  // GALENA_TEXTURE2D_HPP