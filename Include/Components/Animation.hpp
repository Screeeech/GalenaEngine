#ifndef GALENA_ANIMATION_H
#define GALENA_ANIMATION_H
#include <SDL3/SDL_rect.h>

#include <memory>
#include <unordered_map>

#include "Renderable.hpp"
#include "Texture2D.hpp"

namespace gla
{

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

class Animation : public Renderable
{
public:
    explicit Animation(GameObject* pOwner, int zIndex = 0);
    ~Animation() noexcept override = default;

    Animation(Animation const&) = delete;
    auto operator=(Animation const&) -> Animation& = delete;
    Animation(Animation&&) = delete;
    auto operator=(Animation&&) -> Animation& = delete;

    void Update(float deltaTime) override;
    void Render() override;

    void SetPlaying(bool playing);

    auto AddSpriteSheet(std::shared_ptr<Texture2D> const& texture, int cols, int rows) -> SpriteSheet&;
    void AddAnimation(unsigned int animationID, SpriteSheet& spriteSheet, std::initializer_list<FrameData> frameData);

    void SetActiveAnimation(unsigned int animationID, bool startPlaying = false);

private:
    auto GetActiveFrame() const -> const Frame*;
    void AdvanceFrame();

    std::vector<SpriteSheet> m_spriteSheets;
    std::unordered_map<unsigned int, std::vector<Frame>> m_animations;

    float m_elapsedTime{};
    unsigned int m_currentAnimation{};
    size_t m_frameIndex{};
    bool m_playing{};
};

}  // namespace gla

#endif  // GALENA_ANIMATION_H
