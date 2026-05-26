#include "Components/Animation.hpp"

#include <cstdint>
#include <stdexcept>

#include "GameObject.hpp"
#include "Locator.hpp"
#include "Services/Renderer.hpp"

namespace gla
{

Animation::Animation(GameObject* pOwner, int zIndex)
    : Renderable(pOwner, zIndex)
{
}

void Animation::Update(float deltaTime)
{
    if (not m_playing)
        return;

    auto const* frame = GetActiveFrame();

    // If duration is 0, it should be active for one frame maybe?
    if (frame->duration <= 0.f)
    {
        AdvanceFrame();
        return;
    }

    m_elapsedTime += deltaTime;
    while (m_elapsedTime >= frame->duration)
    {
        m_elapsedTime -= frame->duration;
        AdvanceFrame();
    }
}

void Animation::Render()
{
    auto const* frame = GetActiveFrame();
    if (not frame)
        return;

    const glm::vec3 worldPos{ m_pOwner->GetWorldPosition() };
    const glm::vec2 scale{ m_pOwner->GetTransform().GetWorldScale() };

    Locator::Get<Renderer>().RenderTextureScaleFlipped(
        *frame->spriteSheet->texture,
        worldPos.x,
        worldPos.y,
        scale.x,
        scale.y,
        frame->flipX,
        frame->flipY,
        frame->srcRect);
}

void Animation::SetPlaying(bool playing)
{
    m_playing = playing;
}

auto Animation::AddSpriteSheet(std::shared_ptr<Texture2D> const& texture, int cols, int rows) -> SpriteSheet&
{
    assert(texture != nullptr && "Texture cannot be null");
    return m_spriteSheets.emplace_back(texture, cols, rows);
}

void Animation::AddAnimation(uint32_t animationID, SpriteSheet& spriteSheet, std::initializer_list<FrameData> frameData)
{
    if (m_animations.contains(animationID))
        return;

    const glm::vec2 size{ spriteSheet.texture->GetSize() };
    const float width{ size.x / static_cast<float>(spriteSheet.cols) };
    const float height{ size.y / static_cast<float>(spriteSheet.rows) };

    std::vector<Frame> frames;
    frames.reserve(frames.size());
    for (const auto& frame : frameData)
    {
        const SDL_FRect srcRect{ .x = static_cast<float>(frame.colIdx) * width,
            .y = static_cast<float>(frame.rowIdx) * height,
            .w = width,
            .h = height };
        frames.emplace_back(&spriteSheet, frame.duration, srcRect, frame.flipX, frame.flipY);
    }

    m_animations.emplace(animationID, std::move(frames));
}

void Animation::SetAnimation(uint32_t animationID, bool startPlaying)
{
    if (not m_animations.contains(animationID))
        throw std::runtime_error("Animation not found");

    // Lets us set active animation over and over without resetting the frame
    if (m_currentAnimation == animationID)
    {
        SetPlaying(startPlaying);
        return;
    }

    m_currentAnimation = animationID;
    m_frameIndex = 0;
    m_elapsedTime = 0.0f;
    SetPlaying(startPlaying);
}

void Animation::SetFrame(uint32_t frameIndex, bool startPlaying)
{
    if (m_animations.at(m_currentAnimation).size() <= frameIndex)
    {
        std::println("Framed index out of bounds");
        return;
    }

    m_frameIndex = frameIndex;
    m_elapsedTime = 0.0f;
    SetPlaying(startPlaying);
}

auto Animation::GetActiveFrame() const -> Frame const*
{
    if (m_animations.empty())
        return nullptr;

    return &m_animations.at(m_currentAnimation).at(m_frameIndex);
}

void Animation::AdvanceFrame()
{
    ++m_frameIndex;
    if (m_frameIndex >= m_animations.at(m_currentAnimation).size())
        m_frameIndex = 0;
}

}  // namespace gla