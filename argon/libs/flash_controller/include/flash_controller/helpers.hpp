#include <cstdint>

constexpr std::size_t operator"" _KB(unsigned long long int value)
{
    return value * 1024;
}

constexpr std::size_t operator"" _MB(unsigned long long int value)
{
    return value * 1024 * 1024;
}