#include "Components/Timer.hpp"

#include "Time.hpp"

namespace gla
{

Timer::Timer(GameObject* pOwner)
    : Component(pOwner)
{
}

void Timer::Start(float limit)
{
    m_elapsedTime = 0;
    m_timeLimit = limit;
    m_running = true;
}

void Timer::Resume()
{
    if (m_elapsedTime < m_timeLimit)
        m_running = true;
}

void Timer::Pause()
{
    m_running = false;
}

void Timer::Reset()
{
    m_elapsedTime = 0;
    m_running = false;
}

auto Timer::IsRunning() const -> bool
{
    return m_running;
}

auto Timer::IsFinished() const -> bool
{
   return m_elapsedTime >= m_timeLimit;
}

void Timer::LateUpdate()
{
    if (not m_running)
        return;

    m_elapsedTime += Time::Get().DeltaTime();

    if (m_elapsedTime >= m_timeLimit)
        m_running = false;
}

}  // namespace gla