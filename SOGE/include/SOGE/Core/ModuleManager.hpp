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

        eastl::hash_map<TypeKeyPtr, UniquePtr<Module>> m_modules;

    public:
        template <DerivedFromModule T, typename... Args>
        T& CreateModule(Args&&... args)
        {
            const auto key = TypeKey<T>;
            LazyConvertInvoke lazyValue(
                [&args...]() -> UniquePtr<Module> { return CreateUnique<T>(std::forward<Args>(args)...); });

            auto& value = m_modules.try_emplace(key, lazyValue).first->second;
            return dynamic_cast<T&>(*value);
        }

        template <DerivedFromModule T>
        T* GetModule() const
        {
            const auto key = TypeKey<T>;
            auto iter = m_modules.find(key);
            if (iter == m_modules.end())
            {
                return nullptr;
            }

            auto& value = iter->second;
            return dynamic_cast<T*>(value.get());
        }
    };
}

#endif // SOGE_CORE_MODULEMANAGER_HPP
