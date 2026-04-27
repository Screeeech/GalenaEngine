#ifndef GALENA_SINGLETON_HPP
#define GALENA_SINGLETON_HPP

namespace gla
{

template<typename T>
class Singleton
{
public:
    [[nodiscard]] static T& Get()
    {
        static T instance{};
        return instance;
    }

    virtual ~Singleton() = default;
    Singleton(const Singleton& other) = delete;
    Singleton(Singleton&& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;
    Singleton& operator=(Singleton&& other) = delete;

protected:
    Singleton() = default;
};

}  // namespace gla

#endif  // GALENA_SINGLETON_HPP