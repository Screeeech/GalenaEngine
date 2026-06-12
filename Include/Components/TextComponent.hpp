#ifndef GALENA_TEXTCOMPONENT_H
#define GALENA_TEXTCOMPONENT_H

#include <SDL3/SDL_pixels.h>

#include <glm/vec2.hpp>
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
    enum class Align : std::uint8_t
    {
        Left,
        Right,
        Center
    };

    explicit TextComponent(
        GameObject* pOwner,
        std::string text,
        std::shared_ptr<Font> font,
        int zIndex = 0,
        Align alignment = Align::Left,
        SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 });

    void SetText(std::string const& text);
    [[nodiscard]] auto GetText() const -> std::string const&;
    void SetYOffset(float yOffset) const;

protected:
    void OnActivate() override;

    void Update() override;

private:
    std::shared_ptr<Font> m_Font;
    std::shared_ptr<Texture2D> m_TextTexture;
    Align m_alignment;
    SDL_Color m_Color;
    std::string m_Text;
    int m_zIndex;
    Sprite* m_pTextSprite{};

    bool m_NeedsUpdate{ false };

    [[nodiscard]] auto UpdateTexture() const -> std::shared_ptr<Texture2D>;
};

}  // namespace gla

#endif  // GALENA_TEXTCOMPONENT_H
