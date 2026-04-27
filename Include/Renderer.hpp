#ifndef GALENA_RENDERER_HPP
#define GALENA_RENDERER_HPP

#include <SDL3/SDL.h>

#include "Singleton.hpp"

namespace gla
{
class Texture2D;

class Renderer final : public Singleton<Renderer>
{
    SDL_Renderer* m_renderer{};
    SDL_Window* m_window{};
    SDL_Color m_clearColor{};

public:
    void Init(SDL_Window* window);
    void Render() const;
    void Destroy();

    void RenderTexture(const Texture2D& texture, float x, float y, SDL_FRect srcRect = { 0.f, 0.f, -1.f, -1.f }) const;
    void RenderTextureScale(const Texture2D& texture, float x, float y, float scaleX, float scaleY,
                            SDL_FRect srcRect = { 0.f, 0.f, -1.f, -1.f }) const;
    void RenderTextureScaleFlipped(const Texture2D& texture, float x, float y, float scaleX, float scaleY, bool flipX, bool flipY,
                                   SDL_FRect srcRect = { 0.f, 0.f, -1.f, -1.f }) const;

    [[nodiscard]] auto GetSDLRenderer() const -> SDL_Renderer*;

    [[nodiscard]] auto GetBackgroundColor() const -> const SDL_Color&
    {
        return m_clearColor;
    }

    void SetBackgroundColor(const SDL_Color& color)
    {
        m_clearColor = color;
    }
};
}  // namespace gla
#endif  // GALENA_RENDERER_HPP