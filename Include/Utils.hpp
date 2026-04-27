#ifndef GALENA_UTILS_HPP
#define GALENA_UTILS_HPP

namespace gla::utils
{

consteval auto sdbm_hash(const char* str, std::size_t len) -> unsigned int
{
    unsigned int hash = 0;
    for (std::size_t i = 0; i < len; ++i)
    {
        hash = static_cast<unsigned int>(str[i]) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}


}  // namespace gla::utils

consteval auto operator""_h(const char* str, std::size_t len) -> unsigned int
{
    return gla::utils::sdbm_hash(str, len);
}

#endif  // GALENA_UTILS_HPP
