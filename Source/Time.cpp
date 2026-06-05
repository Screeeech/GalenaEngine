#include "Time.hpp"

namespace gla
{

auto Time::DeltaTime() const -> float
{
    return m_deltaTime;
}

auto Time::FixedDeltaTime() const -> float
{
    return m_fixedDeltaTime;
}

void Time::Update(float fixedTime)
{
    m_fixedDeltaTime = fixedTime;

    static auto m_lastTime{ std::chrono::high_resolution_clock::now() };
    auto const now{ std::chrono::high_resolution_clock::now() };
    m_deltaTime = std::chrono::duration<float>(now - m_lastTime).count();
    m_lastTime = now;
}


}  // namespace gla