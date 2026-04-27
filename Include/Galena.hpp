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
    explicit Galena(const std::filesystem::path& dataPath);
    ~Galena() noexcept;
    void Run(const std::function<void()>& load);
    void RunOneFrame();

    Galena(Galena const& other) = delete;
    auto operator=(Galena const& other) -> Galena& = delete;
    Galena(Galena&& other) = delete;
    auto operator=(Galena&& other) -> Galena& = delete;

private:
    bool m_quit{};
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
};

}  // namespace gla

#endif  // GALENA_HPP