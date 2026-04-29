#ifndef GALENA_SERVICELOCATOR_HPP
#define GALENA_SERVICELOCATOR_HPP

#include <any>
#include <memory>
#include <optional>
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
    template<typename ServiceType>
    static void CustomDeleter(void* service)
    {
        delete static_cast<ServiceType*>(service);
    }

    template<typename ServiceType, typename DerivedService = ServiceType, typename... Args>
        requires std::derived_from<DerivedService, ServiceType>
    static void Provide(Args... args)
    {
        if (m_services.contains(typeid(ServiceType)))
            std::println("Careful!  Service of type already exists, providing a new service of this type will overwrite the previous one");

        m_services.erase(typeid(ServiceType));

        // Works when calling Provide with parameters
        m_services.emplace(
            typeid(ServiceType),
            std::unique_ptr<void, void (*)(void*)>(new DerivedService(std::forward<Args>(args)...), &CustomDeleter<ServiceType>));

        // Doesn't work for some reason??
        // m_services[typeid(ServiceType)] = std::unique_ptr<void, void(*)(void*)> (
        //    new ServiceType(std::forward<Args>(args)...),
        //    &CustomDeleter<ServiceType>
        //);
    }

    template<typename ServiceType>
    static auto Request() -> std::optional<ServiceType*>
    {
        auto const it = m_services.find(typeid(ServiceType));
        if (it == m_services.end())
            return std::nullopt;

        return static_cast<ServiceType*>(it->second.get());
    }

    template<typename ServiceType>
    static void Destroy()
    {
        m_services.erase(typeid(ServiceType));
    }

private:
    inline static std::unordered_map<std::type_index, std::unique_ptr<void, void (*)(void*)>> m_services;
};

}  // namespace gla

#endif  // GALENA_SERVICELOCATOR_HPP
