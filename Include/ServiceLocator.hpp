#ifndef GALENA_SERVICELOCATOR_HPP
#define GALENA_SERVICELOCATOR_HPP

#include <any>
#include <memory>
#include <print>
#include <typeindex>
#include <unordered_map>

#include "Singleton.hpp"

namespace gla
{

// TODO: Ask how we can store something that isn't copy constructable
class ServiceLocator final
{
public:
    template<typename ServiceType, typename... Args>
    static void Provide(Args... args)
    {
        if (m_services.contains(typeid(ServiceType)))
            std::println("Careful!  Service of type already exists, providing a new service of this type will overwrite the previous one");

        m_services[typeid(ServiceType)] = std::make_any<ServiceType>(args...);
    }

    template<typename ServiceType>
    static auto Request() -> std::optional<ServiceType*>
    {
        auto const it = m_services.find(typeid(ServiceType));
        if (it == m_services.end())
            return std::nullopt;

        return std::any_cast<ServiceType>(&it->second);
    }

    template<typename ServiceType>
    static void Destroy()
    {
        m_services.erase(typeid(ServiceType));
    }

private:
    inline static std::unordered_map<std::type_index, std::any> m_services;
};

}  // namespace gla

#endif  // GALENA_SERVICELOCATOR_HPP
