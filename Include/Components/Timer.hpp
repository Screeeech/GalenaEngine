#ifndef GALENA_TIMER_HPP
#define GALENA_TIMER_HPP
#include "Component.hpp"
#include <functional>

namespace gla
{

class Timer final : public Component
{
public:
    explicit Timer(GameObject* pOwner, std::function<void()> callback = {});

    void Start(float limit);
    void Resume();
    void Pause();
    void Reset();

    [[nodiscard]] auto IsRunning() const -> bool;
    [[nodiscard]] auto IsFinished() const -> bool;

    void SetCallback(std::function<void()> callback);

protected:
    void LateUpdate() override;

private:
    bool m_running{};
    float m_elapsedTime{};
    float m_timeLimit{};
    std::function<void()> m_timerFinishedCallback;
};

}  // namespace gla

#endif  // BURGERTIME_TIMER_HPP
