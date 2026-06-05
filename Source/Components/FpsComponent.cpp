#include "Components/FpsComponent.hpp"

#include <print>

#include "Components/TextComponent.hpp"
#include "GameObject.hpp"
#include "Time.hpp"

namespace gla
{

FpsComponent::FpsComponent(GameObject* pOwner, std::shared_ptr<Font> font, SDL_Color color)
    : Component(pOwner)
    , m_font(std::move(font))
    , m_color(color)
{
    m_pTextComponent = m_pOwner->AddComponent<TextComponent>("FPS: ", std::move(m_font), 999, m_color);
}

void FpsComponent::Update()
{
    m_elapsedTime += Time::Get().DeltaTime();
    ++m_frameCount;

    if (m_elapsedTime >= 1.f)
    {
        const float fps{ static_cast<float>(m_frameCount) / m_elapsedTime };

        m_pTextComponent->SetText(std::format("FPS: {:.2f}", fps));
        m_frameCount = 0;
        m_elapsedTime = 0.0f;
    }
}

}  // namespace gla
