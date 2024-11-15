#ifndef SOGE_CORE_MODULEMANAGER_HPP
#define SOGE_CORE_MODULEMANAGER_HPP

#include "SOGE/Core/Module.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Utils/Lazy.hpp"

#include <EASTL/hash_map.h>


namespace soge
{
    class ModuleManager
    {
    private:
        template <typename T>
        static constexpr void TypeKey() noexcept
        {
        }
        using TypeKeyPtr = std::add_pointer_t<constexpr void() noexcept>;

        eastl::hash_map<TypeKeyPtr, SharedPtr<Module>> m_modules;

    public:
        template <DerivedFromModule T, typename... Args>
        T& CreateModule(Args&&... args)
        {
            const auto key = TypeKey<T>;
            LazyConvertInvoke lazyValue(
                [&]() -> SharedPtr<Module> { return CreateShared<T>(std::forward<Args>(args)...); });

            auto [_, value] = *m_modules.try_emplace(key, lazyValue).first;
            return dynamic_cast<T&>(*value);
        }
    };
}

#endif // SOGE_CORE_MODULEMANAGER_HPP
