#ifndef SOGE_CORE_MODULEMANAGER_HPP
#define SOGE_CORE_MODULEMANAGER_HPP

#include "SOGE/Core/Module.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Utils/Lazy.hpp"

#include <EASTL/hash_map.h>
#include <EASTL/list.h>
#include <ranges>
#include <typeindex>


template <>
struct eastl::hash<std::type_index>
{
    std::size_t operator()(const std::type_index& aIndex) const
    {
        constexpr std::hash<std::type_index> hash;
        return hash(aIndex);
    }
};

namespace soge
{
    class ModuleManager
    {
    private:
        using Key = std::type_index;
        using UniqueModule = UniquePtr<Module>;

        using Modules = eastl::hash_map<Key, UniqueModule>;
        Modules m_modules;

        using ModulesOrder = eastl::list<Key>;
        ModulesOrder m_modulesOrder;

    public:
        class Iterator;
        class ConstIterator;

        template <DerivedFromModule T, typename... Args>
        eastl::pair<T&, bool> CreateModule(Args&&... args);

        template <DerivedFromModule T, typename... Args>
        eastl::pair<T&, UniquePtr<T>> RecreateModule(Args&&... args);

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
    eastl::pair<T&, bool> ModuleManager::CreateModule(Args&&... args)
    {
        const Key key = typeid(T);

        LazyConvertInvoke lazyModule(
            [&args...]() -> UniqueModule { return CreateUnique<T>(std::forward<Args>(args)...); });

        auto [iter, created] = m_modules.try_emplace(key, lazyModule);
        if (created)
        {
            m_modulesOrder.push_back(key);
        }

        return {dynamic_cast<T&>(*iter->second), created};
    }

    template <DerivedFromModule T, typename... Args>
    eastl::pair<T&, UniquePtr<T>> ModuleManager::RecreateModule(Args&&... args)
    {
        const Key key = typeid(T);

        UniquePtr<T> oldModule;
        UniquePtr<T> newModule = CreateUnique<T>(std::forward<Args>(args)...);
        T* newModulePtr = newModule.get();

        if (const auto iter = m_modules.find(key); iter != m_modules.end())
        {
            oldModule.reset(dynamic_cast<T*>(iter->second.release()));
            iter->second = std::move(newModule);
        }
        else
        {
            m_modules[key] = std::move(newModule);
            m_modulesOrder.push_back(key);
        }

        return {*newModulePtr, std::move(oldModule)};
    }

    template <DerivedFromModule T>
    UniquePtr<T> ModuleManager::RemoveModule()
    {
        const Key key = typeid(T);

        const auto iter = m_modules.find(key);
        if (iter == m_modules.end())
        {
            return UniquePtr<T>{};
        }

        auto module = std::move(iter->second);
        m_modules.erase(iter);
        m_modulesOrder.remove(key);

        T* valuePtr = dynamic_cast<T*>(module.release());
        return UniquePtr<T>{valuePtr};
    }

    template <DerivedFromModule T>
    T* ModuleManager::GetModule() const
    {
        const Key key = typeid(T);

        const auto iter = m_modules.find(key);
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
        friend ModuleManager;
        using KeyIter = ModulesOrder::iterator;

        explicit Iterator(KeyIter aKeyIter, ModuleManager& aManager) noexcept;

        KeyIter m_keyIter;
        ModuleManager* m_manager;

    public:
        // NOLINTBEGIN(readability-identifier-naming)
        using difference_type = KeyIter::difference_type;
        using value_type = Module;
        using pointer = Module*;
        using reference = Module&;
        using iterator_category = std::bidirectional_iterator_tag;
        // NOLINTEND(readability-identifier-naming)

        explicit Iterator() noexcept = default;

        reference operator*() const noexcept;
        pointer operator->() const noexcept;

        Iterator& operator++() noexcept;
        Iterator operator++(int) noexcept;

        Iterator& operator--() noexcept;
        Iterator operator--(int) noexcept;

        friend bool operator==(const Iterator&, const Iterator&) noexcept = default;
    };

    class ModuleManager::ConstIterator
    {
    private:
        friend ModuleManager;
        using KeyIter = ModulesOrder::const_iterator;

        explicit ConstIterator(KeyIter aKeyIter, const ModuleManager& aManager) noexcept;

        KeyIter m_keyIter;
        const ModuleManager* m_manager;

    public:
        // NOLINTBEGIN(readability-identifier-naming)
        using difference_type = KeyIter::difference_type;
        using value_type = Module;
        using pointer = const Module*;
        using reference = const Module&;
        using iterator_category = std::bidirectional_iterator_tag;
        // NOLINTEND(readability-identifier-naming)

        explicit ConstIterator() noexcept = default;

        reference operator*() const noexcept;
        pointer operator->() const noexcept;

        ConstIterator& operator++() noexcept;
        ConstIterator operator++(int) noexcept;

        ConstIterator& operator--() noexcept;
        ConstIterator operator--(int) noexcept;

        friend bool operator==(const ConstIterator&, const ConstIterator&) noexcept = default;
    };


    static_assert(std::bidirectional_iterator<ModuleManager::Iterator>);
    static_assert(std::bidirectional_iterator<ModuleManager::ConstIterator>);

    static_assert(std::ranges::bidirectional_range<ModuleManager>);
}

#endif // SOGE_CORE_MODULEMANAGER_HPP
