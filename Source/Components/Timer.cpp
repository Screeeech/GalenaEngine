#include "Components/Timer.hpp"

#include "Time.hpp"

namespace gla
{

Timer::Timer(GameObject* pOwner, std::function<void()> callback)
    : Component(pOwner)
    , m_timerFinishedCallback(std::move(callback))
{
}

void Timer::Start(float limit)
{
    m_elapsedTime = 0;
    m_timeLimit = limit;
    m_running = true;
}

void Timer::Start(float limit, std::function<void()> oneTimeCallback)
{
    Start(limit);
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
}

auto Timer::IsRunning() const -> bool
{
    return m_running;
}

auto Timer::IsFinished() const -> bool
{
    return m_elapsedTime >= m_timeLimit;
}

void Timer::SetCallback(std::function<void()> callback)
{
    m_timerFinishedCallback = std::move(callback);
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
        if (m_timerFinishedCallback)
            m_timerFinishedCallback();
    }
}

}  // namespace gla