#ifndef GALENA_SINGLETON_HPP
#define GALENA_SINGLETON_HPP

namespace gla
{

template<typename T>
class Singleton
{
public:
    [[nodiscard]] static auto Get() -> T&
    {
        static T instance{};
        return instance;
    }

    virtual ~Singleton() = default;

    Singleton(Singleton const& other) = delete;
    auto operator=(Singleton const& other) -> Singleton& = delete;
    Singleton(Singleton&& other) = delete;
    auto operator=(Singleton&& other) -> Singleton& = delete;

protected:
    Singleton() = default;
};

}  // namespace gla

#endif  // GALENA_SINGLETON_HPP