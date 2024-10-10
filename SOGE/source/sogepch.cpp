#include "sogepch.hpp"


/**
* @brief EASTL new[] operator overloading.
* @details new[] operator overloading required by EASTL library.
* @returns The uint8_t array of given size.
* @note This overload strongly required by EASTL library and must be
* located in global space. Note that pch header may be probably not
* a very good place to store it.
**/
void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
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
void* __cdecl operator new[](size_t size, size_t a, size_t b, const char* name,
    int flags, unsigned debugFlags, const char* file, int line)
{
    return new uint8_t[size];
}