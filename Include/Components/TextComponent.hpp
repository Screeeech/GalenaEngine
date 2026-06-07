#ifndef GALENA_TEXTCOMPONENT_H
#define GALENA_TEXTCOMPONENT_H

#include <SDL3/SDL_pixels.h>

#include <memory>

#include "Component.hpp"
#include "Font.hpp"

namespace gla
{
class Sprite;
class Texture2D;

class TextComponent : public Component
{
public:
    explicit TextComponent(
        GameObject* pOwner,
        std::string text,
        std::shared_ptr<Font> font,
        int zIndex = 0,
        SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 });

    void SetText(std::string const& text);
    [[nodiscard]] auto GetText() const -> std::string const&;

protected:
    void OnActivate() override;

    void Update() override;

private:
    std::shared_ptr<Font> m_Font;
    std::shared_ptr<Texture2D> m_TextTexture;
    SDL_Color m_Color;
    std::string m_Text;
    int m_zIndex;

    bool m_NeedsUpdate{ false };

    Sprite* m_pRenderComponent{};

    [[nodiscard]] auto UpdateTexture() const -> std::shared_ptr<Texture2D>;
};

}  // namespace gla

#endif  // GALENA_TEXTCOMPONENT_H
