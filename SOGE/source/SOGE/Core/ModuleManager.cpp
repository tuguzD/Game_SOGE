#include "sogepch.hpp"

#include "SOGE/Core/ModuleManager.hpp"


namespace soge
{
    auto ModuleManager::begin() noexcept -> Iterator
    {
        return Iterator{m_modules.begin()};
    }

    auto ModuleManager::begin() const noexcept -> ConstIterator
    {
        return ConstIterator{m_modules.begin()};
    }

    auto ModuleManager::end() noexcept -> Iterator
    {
        return Iterator{m_modules.end()};
    }

    auto ModuleManager::end() const noexcept -> ConstIterator
    {
        return ConstIterator{m_modules.end()};
    }

    ModuleManager::Iterator::Iterator(const Base aIter) noexcept : m_iter(aIter)
    {
    }

    auto ModuleManager::Iterator::operator*() const noexcept -> reference
    {
        return m_iter->second.operator*();
    }

    auto ModuleManager::Iterator::operator->() const noexcept -> pointer
    {
        return m_iter->second.operator->();
    }

    auto ModuleManager::Iterator::operator++() noexcept -> Iterator&
    {
        ++m_iter;
        return *this;
    }

    auto ModuleManager::Iterator::operator++(int) noexcept -> Iterator
    {
        const auto temp = *this;
        ++m_iter;
        return temp;
    }

    ModuleManager::ConstIterator::ConstIterator(const Base aIter) noexcept : m_iter(aIter)
    {
    }

    auto ModuleManager::ConstIterator::operator*() const noexcept -> reference
    {
        return m_iter->second.operator*();
    }

    auto ModuleManager::ConstIterator::operator->() const noexcept -> pointer
    {
        return m_iter->second.operator->();
    }

    auto ModuleManager::ConstIterator::operator++() noexcept -> ConstIterator&
    {
        ++m_iter;
        return *this;
    }

    auto ModuleManager::ConstIterator::operator++(int) noexcept -> ConstIterator
    {
        const auto temp = *this;
        ++m_iter;
        return temp;
    }
}
