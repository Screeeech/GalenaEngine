#ifndef GALENA_ANIMATION_H
#define GALENA_ANIMATION_H
#include <SDL3/SDL_rect.h>

#include <memory>
#include <unordered_map>

#include "Renderable.hpp"
#include "Texture2D.hpp"

using AnimationID = uint32_t;

namespace gla
{

class GameObject;

struct SpriteSheet final
{
    std::shared_ptr<Texture2D> texture;
    int cols{};
    int rows{};
};

struct Frame final
{
    SpriteSheet* spriteSheet{};
    float duration{};
    SDL_FRect srcRect{};
    bool flipX{};
    bool flipY{};
};

struct FrameData final
{
    int colIdx{};
    int rowIdx{};
    float duration{};
    bool flipX{};
    bool flipY{};
};

class Animation final : public Renderable
{
public:
    explicit Animation(GameObject* pOwner, int zIndex = 0);
    ~Animation() noexcept override = default;

    void SetPlaying(bool playing);
    auto IsPlaying() const -> bool;

    auto AddSpriteSheet(std::shared_ptr<Texture2D> const& texture, int cols, int rows) -> SpriteSheet&;
    void AddAnimation(AnimationID animationID, SpriteSheet& spriteSheet, std::initializer_list<FrameData> frameData);

    void SetAnimation(AnimationID animationID, bool startPlaying = false, bool looping = true);
    void SetFrame(AnimationID frameIndex, bool startPlaying = false, bool looping = true);

    Animation(Animation const&) = delete;
    auto operator=(Animation const&) -> Animation& = delete;
    Animation(Animation&&) = delete;
    auto operator=(Animation&&) -> Animation& = delete;

protected:
    void Update() override;
    void Render() override;

private:
    auto GetActiveFrame() const -> const Frame*;
    void AdvanceFrame();

    std::vector<SpriteSheet> m_spriteSheets;
    std::unordered_map<uint32_t, std::vector<Frame>> m_animations;

    float m_elapsedTime{};
    AnimationID m_currentAnimation{};
    size_t m_frameIndex{};
    bool m_playing{};
    bool m_looping{ true };
};

}  // namespace gla

#endif  // GALENA_ANIMATION_H
