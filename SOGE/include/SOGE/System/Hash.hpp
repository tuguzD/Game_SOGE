#ifndef SOGE_SYSTEM_HASH_HPP
#define SOGE_SYSTEM_HASH_HPP

#include <limits>


namespace soge
{
    // NOLINTBEGIN(readability-identifier-naming) reason: to better represent the fact that it was borrowed from Boost:
    // https://github.com/boostorg/container_hash/blob/89e5b98f6bc05841a21069d76cc5adcbee62b9cc/include/boost/container_hash/detail/hash_mix.hpp
    constexpr std::size_t MixHash(std::size_t x)
    // NOLINTEND(readability-identifier-naming)
    {
        constexpr auto digits = std::numeric_limits<std::size_t>::digits;
        static_assert(digits == 64 || digits == 32, "16-bit and 8-bit architectures are not supported");

        if constexpr (digits == 64)
        {
            constexpr std::uint64_t m = 0xe9846af9b1a615d;

            x ^= x >> 32;
            x *= m;
            x ^= x >> 32;
            x *= m;
            x ^= x >> 28;

            return x;
        }
        else
        {
            constexpr std::uint32_t m1 = 0x21f0aaad;
            constexpr std::uint32_t m2 = 0x735a2d97;

            x ^= x >> 16;
            x *= m1;
            x ^= x >> 15;
            x *= m2;
            x ^= x >> 15;

            return x;
        }
    }

    // This was borrowed from Boost, too:
    // https://github.com/boostorg/container_hash/blob/89e5b98f6bc05841a21069d76cc5adcbee62b9cc/include/boost/container_hash/hash.hpp#L469
    template <template <typename> typename Hash = std::hash, typename T, typename... Rest>
    constexpr void HashCombine(std::size_t& aHash, const T& aValue, const Rest&... aRest)
    {
        Hash<T> hasher;
        aHash = MixHash(aHash + 0x9e3779b9 + hasher(aValue));
        (HashCombine<Hash>(aHash, aRest), ...);
    }
}

#endif // SOGE_SYSTEM_HASH_HPP
