#include "sogepch.hpp"

/**
 * @brief EASTL new[] operator overloading.
 * @details new[] operator overloading required by EASTL library.
 * @returns The uint8_t array of given size.
 * @note This overload strongly required by EASTL library and must be
 * located in global space. Note that pch header may be probably not
 * a very good place to store it.
 **/
// NOLINTBEGIN(readability-identifier-naming)
void* __cdecl operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
// NOLINTEND(readability-identifier-naming)
{
    return new uint8_t[size];
}

/**
 * @brief EASTL new[] operator overloading.
 * @details new[] operator overloading required by EASTL library.
 * @returns The uint8_t array of given size.
 * @note This overload strongly required by EASTL library and must be
 * located in global space. Note that pch header may be probably not
 * a very good place to store it.
 **/
// NOLINTBEGIN(readability-identifier-naming)
void* __cdecl operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags,
                             unsigned debugFlags, const char* file, int line)
// NOLINTEND(readability-identifier-naming)
{
    return new uint8_t[size];
}
