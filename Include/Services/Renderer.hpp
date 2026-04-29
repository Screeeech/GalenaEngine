#ifndef GALENA_RENDERER_HPP
#define GALENA_RENDERER_HPP

#include <SDL3/SDL.h>

#include <glm/vec2.hpp>
#include <initializer_list>
#include <utility>

namespace gla
{
class Texture2D;

class Renderer final
{
    SDL_Renderer* m_renderer{};
    SDL_Window* m_window{};
    SDL_Color m_clearColor{};
    SDL_Point m_logicalResolution{};

public:
    explicit Renderer(SDL_Window* pWindow);
    ~Renderer() noexcept;

    Renderer(Renderer const&) = delete;
    Renderer(Renderer&&) noexcept = delete;
    auto operator=(Renderer const&) -> Renderer& = delete;
    auto operator=(Renderer&&) noexcept -> Renderer& = delete;

    void Render() const;

    void SetLogicalResolution(int width, int height, SDL_RendererLogicalPresentation mode = SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

    void RenderTexture(Texture2D const& texture, float x, float y, SDL_FRect srcRect = { .x = 0.f, .y = 0.f, .w = -1.f, .h = -1.f }) const;
    void RenderTextureScale(
        const Texture2D& texture, float x, float y, float scaleX, float scaleY, SDL_FRect srcRect = { .x = 0.f, .y = 0.f, .w = -1.f, .h = -1.f })
        const;
    void RenderTextureScaleFlipped(
        const Texture2D& texture,
        float x,
        float y,
        float scaleX,
        float scaleY,
        bool flipX,
        bool flipY,
        SDL_FRect srcRect = { .x = 0.f, .y = 0.f, .w = -1.f, .h = -1.f }) const;

    void SetColor(SDL_Color color) const;
    void DrawLines(std::initializer_list<std::tuple<glm::vec2, glm::vec2, SDL_Color>> lines) const;
    void DrawLines(std::initializer_list<std::tuple<glm::vec2, glm::vec2>> lines) const;
    void DrawRect(SDL_FRect rect) const;

    [[nodiscard]] auto GetSDLRenderer() const -> SDL_Renderer* { return m_renderer; }
    [[nodiscard]] auto GetBackgroundColor() const -> const SDL_Color& { return m_clearColor; }

    void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
};
}  // namespace gla
#endif  // GALENA_RENDERER_HPP