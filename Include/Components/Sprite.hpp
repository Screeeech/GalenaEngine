#ifndef GALENA_SPRITE_H
#define GALENA_SPRITE_H
#include <SDL3/SDL_rect.h>

#include <memory>

#include "Renderable.hpp"

namespace gla
{
class Texture2D;
class GameObject;

class Sprite : public Renderable
{
public:
    explicit Sprite(GameObject* pOwner, std::shared_ptr<Texture2D> texture, int zIndex = 0);
    explicit Sprite(GameObject* pOwner, int zIndex = 0);


    void SetTexture(std::shared_ptr<Texture2D> texture);
    void SetSourceRect(SDL_FRect sourceRect);
    void SetSourceRect(float x, float y, float w, float h);
    void SetSourceRectPos(float x, float y);
    void SetSourceRectSize(float w, float h);

    [[nodiscard]] auto GetSourceRect() const -> SDL_FRect;

    bool m_Visible{ true };

protected:
    void Render() override;

private:
    std::shared_ptr<Texture2D> m_texture;
    SDL_FRect m_sourceRect;
};

}  // namespace gla

#endif  // GALENA_SPRITE_H
