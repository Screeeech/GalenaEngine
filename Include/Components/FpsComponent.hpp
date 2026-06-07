#ifndef GALENA_FPSCOMPONENT_H
#define GALENA_FPSCOMPONENT_H

#include <SDL3/SDL_pixels.h>

#include <memory>

#include "Component.hpp"

namespace gla
{
class TextComponent;
class Transform;
class Font;

class FpsComponent : public Component
{
public:
    explicit FpsComponent(
        GameObject* pOwner, std::shared_ptr<Font> font, int zIndex = 999, SDL_Color = { .r = 255, .g = 255, .b = 255, .a = 255 });

protected:
    void Update() override;

private:
    std::shared_ptr<Font> m_font;

    int m_frameCount{};
    float m_elapsedTime{};
    TextComponent* m_pTextComponent;
};

}  // namespace gla

#endif  // GALENA_FPSCOMPONENT_H
