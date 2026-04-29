#ifndef GALENA_UTILS_HPP
#define GALENA_UTILS_HPP

#include <cstdint>

namespace gla::utils
{

consteval auto sdbm_hash(const char* str, std::size_t len) -> std::uint32_t
{
    std::uint32_t hash = 0;
    for (std::size_t i = 0; i < len; ++i)
    {
        hash = static_cast<std::uint32_t>(str[i]) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}


}  // namespace gla::utils

consteval auto operator""_h(const char* str, std::size_t len) -> std::uint32_t
{
    return gla::utils::sdbm_hash(str, len);
}

#endif  // GALENA_UTILS_HPP
