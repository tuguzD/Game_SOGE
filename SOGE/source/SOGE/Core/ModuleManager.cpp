#include "sogepch.hpp"

#include "SOGE/Core/ModuleManager.hpp"


namespace soge
{
    auto ModuleManager::begin() noexcept -> Iterator
    {
        return Iterator{m_modulesOrder.begin(), *this};
    }

    auto ModuleManager::begin() const noexcept -> ConstIterator
    {
        return ConstIterator{m_modulesOrder.begin(), *this};
    }

    auto ModuleManager::end() noexcept -> Iterator
    {
        return Iterator{m_modulesOrder.end(), *this};
    }

    auto ModuleManager::end() const noexcept -> ConstIterator
    {
        return ConstIterator{m_modulesOrder.end(), *this};
    }

    ModuleManager::Iterator::Iterator(const KeyIter aKeyIter, ModuleManager& aManager) noexcept
        : m_keyIter(aKeyIter), m_manager(&aManager)
    {
    }

    auto ModuleManager::Iterator::operator*() const noexcept -> reference
    {
        const auto key = *m_keyIter;
        const auto iter = m_manager->m_modules.find(key);
        return iter->second.operator*();
    }

    auto ModuleManager::Iterator::operator->() const noexcept -> pointer
    {
        const auto key = *m_keyIter;
        const auto iter = m_manager->m_modules.find(key);
        return iter->second.operator->();
    }

    auto ModuleManager::Iterator::operator++() noexcept -> Iterator&
    {
        ++m_keyIter;
        return *this;
    }

    auto ModuleManager::Iterator::operator++(int) noexcept -> Iterator
    {
        const auto temp = *this;
        ++m_keyIter;
        return temp;
    }

    auto ModuleManager::Iterator::operator--() noexcept -> Iterator&
    {
        --m_keyIter;
        return *this;
    }

    auto ModuleManager::Iterator::operator--(int) noexcept -> Iterator
    {
        const auto temp = *this;
        --m_keyIter;
        return temp;
    }

    ModuleManager::ConstIterator::ConstIterator(const KeyIter aKeyIter, const ModuleManager& aManager) noexcept
        : m_keyIter(aKeyIter), m_manager(&aManager)
    {
    }

    auto ModuleManager::ConstIterator::operator*() const noexcept -> reference
    {
        const auto key = *m_keyIter;
        const auto iter = m_manager->m_modules.find(key);
        return iter->second.operator*();
    }

    auto ModuleManager::ConstIterator::operator->() const noexcept -> pointer
    {
        const auto key = *m_keyIter;
        const auto iter = m_manager->m_modules.find(key);
        return iter->second.operator->();
    }

    auto ModuleManager::ConstIterator::operator++() noexcept -> ConstIterator&
    {
        ++m_keyIter;
        return *this;
    }

    auto ModuleManager::ConstIterator::operator++(int) noexcept -> ConstIterator
    {
        const auto temp = *this;
        ++m_keyIter;
        return temp;
    }

    auto ModuleManager::ConstIterator::operator--() noexcept -> ConstIterator&
    {
        --m_keyIter;
        return *this;
    }

    ModuleManager::ConstIterator ModuleManager::ConstIterator::operator--(int) noexcept
    {
        const auto temp = *this;
        --m_keyIter;
        return temp;
    }
}
