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
    m_oneTimeCallback = nullptr;
}

void Timer::Start(float limit, std::function<void()> oneTimeCallback)
{
    m_elapsedTime = 0;
    m_timeLimit = limit;
    m_running = true;
    m_oneTimeCallback = std::move(oneTimeCallback);
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
    m_oneTimeCallback = nullptr;
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
    {
        m_running = false;
        if (m_oneTimeCallback)
        {
            m_oneTimeCallback();
            m_oneTimeCallback = nullptr;
        }
    }
}

}  // namespace gla