#ifndef GALENA_HPP
#define GALENA_HPP

#include <chrono>
#include <filesystem>
#include <functional>
#include <string>

namespace gla
{
class Galena final
{
public:
    explicit Galena(std::string const& windowName, int fixedUpdateFrameCap = 0);
    ~Galena() noexcept;
    void Run(const std::function<void()>& load);
    void RunOneFrame();

    Galena(Galena const& other) = delete;
    auto operator=(Galena const& other) -> Galena& = delete;
    Galena(Galena&& other) = delete;
    auto operator=(Galena&& other) -> Galena& = delete;

private:
    bool m_quit{};
    float m_lag{};
    float m_fixedTimeStep;
};

}  // namespace gla

#endif  // GALENA_HPP