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
        using UniqueModule = UniquePtr<Module>;

        using Modules = eastl::hash_map<TypeKeyPtr, UniqueModule>;
        Modules m_modules;

    public:
        class Iterator;
        class ConstIterator;

        template <DerivedFromModule T, typename... Args>
        std::pair<T&, bool> CreateModule(Args&&... args);

        template <DerivedFromModule T, typename... Args>
        std::pair<T&, UniquePtr<T>> RecreateModule(Args&&... args);

        template <DerivedFromModule T>
        UniquePtr<T> RemoveModule();

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
    std::pair<T&, bool> ModuleManager::CreateModule(Args&&... args)
    {
        const auto key = TypeKey<T>;
        LazyConvertInvoke lazyModule(
            [&args...]() -> UniqueModule { return CreateUnique<T>(std::forward<Args>(args)...); });

        auto [iter, created] = m_modules.try_emplace(key, lazyModule);
        return {dynamic_cast<T&>(*iter->second), created};
    }

    template <DerivedFromModule T, typename... Args>
    std::pair<T&, UniquePtr<T>> ModuleManager::RecreateModule(Args&&... args)
    {
        const auto key = TypeKey<T>;

        UniquePtr<T> oldModule;
        UniquePtr<T> newModule = CreateUnique<T>(std::forward<Args>(args)...);
        T* newModulePtr = newModule.get();

        if (auto iter = m_modules.find(key); iter != m_modules.end())
        {
            oldModule.reset(dynamic_cast<T*>(iter->second.release()));
            iter->second = std::move(newModule);
        }
        else
        {
            m_modules[key] = std::move(newModule);
        }

        return {*newModulePtr, std::move(oldModule)};
    }

    template <DerivedFromModule T>
    UniquePtr<T> ModuleManager::RemoveModule()
    {
        const auto key = TypeKey<T>;
        auto iter = m_modules.find(key);
        if (iter == m_modules.end())
        {
            return UniquePtr<T>{};
        }

        auto module = std::move(iter->second);
        m_modules.erase(iter);

        T* valuePtr = dynamic_cast<T*>(module.release());
        return UniquePtr<T>{valuePtr};
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

        explicit Iterator() noexcept = default;
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

        explicit ConstIterator() noexcept = default;
        explicit ConstIterator(Base aIter) noexcept;

        reference operator*() const noexcept;
        pointer operator->() const noexcept;

        ConstIterator& operator++() noexcept;
        ConstIterator operator++(int) noexcept;

        friend bool operator==(const ConstIterator&, const ConstIterator&) noexcept = default;
    };


    static_assert(std::forward_iterator<ModuleManager::Iterator>);
    static_assert(std::forward_iterator<ModuleManager::ConstIterator>);

    static_assert(std::ranges::forward_range<ModuleManager>);
}

#endif // SOGE_CORE_MODULEMANAGER_HPP
