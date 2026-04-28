#include "Services/Renderer.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <implot.h>

#include <iostream>
#include <stdexcept>

#include "SceneManager.hpp"
#include "Texture2D.hpp"

namespace gla
{

Renderer::Renderer(SDL_Window* pWindow)
    : m_renderer(SDL_CreateRenderer(pWindow, nullptr))
    , m_window(pWindow)
{
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    if (m_renderer == nullptr)
    {
        std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
        throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
#if __EMSCRIPTEN__
    io.IniFilename = NULL;
#endif

    ImGui_ImplSDL3_InitForSDLRenderer(pWindow, m_renderer);
    ImGui_ImplSDLRenderer3_Init(m_renderer);
}

Renderer::~Renderer() noexcept
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
}

void Renderer::Render() const
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    SceneManager::Get().DrawUI();

    ImGui::Render();

    SetColor(m_clearColor);
    SDL_RenderClear(m_renderer);

    // Draw temporary bounding box around game canvas
    SetColor({ .r = 255, .g = 0, .b = 0, .a = 255 });
    DrawRect({ .x = 0, .y = 0, .w = static_cast<float>(m_logicalResolution.x), .h = static_cast<float>(m_logicalResolution.y) });

    SceneManager::Get().Render();

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
    SDL_RenderPresent(m_renderer);
}

void Renderer::SetLogicalResolution(int width, int height, SDL_RendererLogicalPresentation mode)
{
    SDL_SetRenderLogicalPresentation(m_renderer, width, height, mode);
    m_logicalResolution = { .x = width, .y = height };
}

void Renderer::RenderTexture(const Texture2D& texture, float x, float y, SDL_FRect srcRect) const
{
    SDL_FRect dst{};
    dst.x = x;
    dst.y = y;
    const SDL_FRect* pSrcRect{};
    if (srcRect.w >= 0 and srcRect.h >= 0)
    {
        pSrcRect = &srcRect;
        dst.w = srcRect.w;
        dst.h = srcRect.h;
    }
    else
    {
        SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
    }

    SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), pSrcRect, &dst);
}

void Renderer::RenderTextureScale(const Texture2D& texture, float x, float y, float scaleX, float scaleY, SDL_FRect srcRect) const
{
    SDL_FRect dst{};
    dst.x = x;
    dst.y = y;

    const SDL_FRect* pSrcRect{};
    if (srcRect.w >= 0 and srcRect.h >= 0)
    {
        pSrcRect = &srcRect;
        dst.w = srcRect.w;
        dst.h = srcRect.w;
    }
    else
    {
        SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
    }

    dst.w *= scaleX;
    dst.h *= scaleY;

    SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), pSrcRect, &dst);
}

void Renderer::RenderTextureScaleFlipped(
    Texture2D const& texture, float x, float y, float scaleX, float scaleY, bool flipX, bool flipY, SDL_FRect srcRect) const
{
    SDL_FRect dst{};
    dst.x = x;
    dst.y = y;
    dst.w = srcRect.w * scaleX;
    dst.h = srcRect.w * scaleY;

    const SDL_FRect* pSrcRect{};
    if (srcRect.w >= 0 and srcRect.h >= 0)
        pSrcRect = &srcRect;

    constexpr SDL_FPoint origin = { 0.f, 0.f };

    uint8_t flipBits{};
    if (flipX)
        flipBits |= SDL_FLIP_HORIZONTAL;
    if (flipY)
        flipBits |= SDL_FLIP_VERTICAL;
    const auto flipMode{ static_cast<SDL_FlipMode>(flipBits) };

    SDL_RenderTextureRotated(GetSDLRenderer(), texture.GetSDLTexture(), pSrcRect, &dst, 0, &origin, flipMode);
}

void Renderer::DrawLines(std::initializer_list<std::tuple<glm::vec2, glm::vec2, SDL_Color>> lines) const
{
    for (auto const [start, end, color] : lines)
    {
        SetColor(color);
        SDL_RenderLine(m_renderer, start.x, start.y, end.x, end.y);
    }
}

void Renderer::DrawLines(std::initializer_list<std::tuple<glm::vec2, glm::vec2>> lines) const
{
    for (auto const [start, end] : lines)
    {
        SDL_RenderLine(m_renderer, start.x, start.y, end.x, end.y);
    }
}

void Renderer::DrawRect(SDL_FRect rect) const
{
    SDL_RenderRect(m_renderer, &rect);
}

void Renderer::SetColor(SDL_Color color) const
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
}

}  // namespace gla
