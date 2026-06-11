#ifndef GALENA_TIMER_HPP
#define GALENA_TIMER_HPP
#include <functional>

#include "Component.hpp"

namespace gla
{

class Timer final : public Component
{
public:
    explicit Timer(GameObject* pOwner);

    void Start(float limit);
    void Start(float limit, std::function<void()> oneTimeCallback);
    void Resume();
    void Pause();
    void Reset();

    [[nodiscard]] auto IsRunning() const -> bool;
    [[nodiscard]] auto IsFinished() const -> bool;

protected:
    void LateUpdate() override;

private:
    bool m_running{};
    float m_elapsedTime{};
    float m_timeLimit{};
    std::function<void()> m_oneTimeCallback;
};

}  // namespace gla

#endif  // BURGERTIME_TIMER_HPP
