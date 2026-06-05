#ifndef BURGERTIME_TIME_HPP
#define BURGERTIME_TIME_HPP

#include <chrono>

#include "Singleton.hpp"


namespace gla
{

class Time final : public Singleton<Time>
{
public:
    auto DeltaTime() const -> float;
    auto FixedDeltaTime() const -> float;

    void Update(float fixedTime);

private:

    float m_deltaTime{};
    float m_fixedDeltaTime{};
};

}  // namespace gla

#endif  // BURGERTIME_TIME_HPP
