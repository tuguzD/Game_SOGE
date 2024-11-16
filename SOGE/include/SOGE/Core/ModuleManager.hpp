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
        using UniqueModule = UniquePtr<Module>;

        using Modules = eastl::hash_map<TypeKeyPtr, UniqueModule>;
        Modules m_modules;

    public:
        class Iterator;
        class ConstIterator;

        template <DerivedFromModule T, typename... Args>
        T& CreateModule(Args&&... args);

        template <DerivedFromModule T, typename... Args>
        T& RecreateModule(Args&&... args);

        template <DerivedFromModule T>
        eastl::optional<T> RemoveModule();

        template <DerivedFromModule T>
        [[nodiscard]]
        T* GetModule() const;

        // NOLINTBEGIN(readability-identifier-naming)
        Iterator begin() noexcept;
        ConstIterator begin() const noexcept;

        Iterator end() noexcept;
        ConstIterator end() const noexcept;
        // NOLINTEND(readability-identifier-naming)
    };

    template <DerivedFromModule T, typename... Args>
    T& ModuleManager::CreateModule(Args&&... args)
    {
        const auto key = TypeKey<T>;
        LazyConvertInvoke lazyModule(
            [&args...]() -> UniqueModule { return CreateUnique<T>(std::forward<Args>(args)...); });

        auto& module = m_modules.try_emplace(key, lazyModule).first->second;
        return dynamic_cast<T&>(*module);
    }

    template <DerivedFromModule T, typename... Args>
    T& ModuleManager::RecreateModule(Args&&... args)
    {
        const auto key = TypeKey<T>;

        auto& module = (m_modules[key] = CreateUnique<T>(std::forward<Args>(args)...));
        return dynamic_cast<T&>(*module);
    }

    template <DerivedFromModule T>
    eastl::optional<T> ModuleManager::RemoveModule()
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
    T* ModuleManager::GetModule() const
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

    class ModuleManager::Iterator
    {
    private:
        using Base = Modules::iterator;
        Base m_iter;

    public:
        // NOLINTBEGIN(readability-identifier-naming)
        using difference_type = Base::difference_type;
        using value_type = Module;
        using pointer = Module*;
        using reference = Module&;
        using iterator_category = std::forward_iterator_tag;
        // NOLINTEND(readability-identifier-naming)

        explicit Iterator(Base aIter) noexcept;

        reference operator*() const noexcept;
        pointer operator->() const noexcept;

        Iterator& operator++() noexcept;
        Iterator operator++(int) noexcept;

        friend bool operator==(const Iterator&, const Iterator&) noexcept = default;
    };

    class ModuleManager::ConstIterator
    {
    private:
        using Base = Modules::const_iterator;
        Base m_iter;

    public:
        // NOLINTBEGIN(readability-identifier-naming)
        using difference_type = Base::difference_type;
        using value_type = Module;
        using pointer = const Module*;
        using reference = const Module&;
        using iterator_category = std::forward_iterator_tag;
        // NOLINTEND(readability-identifier-naming)

        explicit ConstIterator(Base aIter) noexcept;

        reference operator*() const noexcept;
        pointer operator->() const noexcept;

        ConstIterator& operator++() noexcept;
        ConstIterator operator++(int) noexcept;

        friend bool operator==(const ConstIterator&, const ConstIterator&) noexcept = default;
    };
}

#endif // SOGE_CORE_MODULEMANAGER_HPP
