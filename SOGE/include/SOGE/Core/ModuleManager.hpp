#ifndef SOGE_CORE_MODULEMANAGER_HPP
#define SOGE_CORE_MODULEMANAGER_HPP

#include "SOGE/Core/Module.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Utils/Lazy.hpp"

#include <EASTL/hash_map.h>
#include <EASTL/optional.h>


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
            LazyConvertInvoke lazyModule(
                [&args...]() -> UniquePtr<Module> { return CreateUnique<T>(std::forward<Args>(args)...); });

            auto& module = m_modules.try_emplace(key, lazyModule).first->second;
            return dynamic_cast<T&>(*module);
        }

        template <DerivedFromModule T>
        eastl::optional<T> RemoveModule()
        {
            const auto key = TypeKey<T>;
            auto iter = m_modules.find(key);
            if (iter == m_modules.end())
            {
                return eastl::nullopt;
            }

            auto module = std::move(iter->second);
            m_modules.erase(iter);

            T* value = dynamic_cast<T*>(module.get());
            return {std::move(*value)};
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

            auto& module = iter->second;
            return dynamic_cast<T*>(module.get());
        }
    };
}

#endif // SOGE_CORE_MODULEMANAGER_HPP
