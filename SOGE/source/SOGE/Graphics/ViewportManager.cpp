#include "sogepch.hpp"

#include "SOGE/Graphics/ViewportManager.hpp"


namespace soge
{
    auto ViewportManager::CreateViewport(const Viewport& aViewport) -> eastl::pair<Viewport&, Key>
    {
        const auto key = UUID::Generate();

        const auto [iter, created] = m_viewports.try_emplace(key, aViewport);
        if (created)
        {
            m_viewportsOrder.push_back(key);
        }

        return {iter->second, key};
    }

    auto ViewportManager::GetViewport(const Key& aKey) -> Viewport*
    {
        const auto iter = m_viewports.find(aKey);
        if (iter == m_viewports.end())
        {
            return nullptr;
        }

        return &iter->second;
    }

    auto ViewportManager::DestroyViewport(const Key& aKey) -> eastl::optional<Viewport>
    {
        const auto iter = m_viewports.find(aKey);
        if (iter == m_viewports.end())
        {
            return eastl::nullopt;
        }

        Viewport viewport = std::move(iter->second);
        m_viewportsOrder.remove(iter->first);
        m_viewports.erase(iter);

        return viewport;
    }

    void ViewportManager::Clear()
    {
        m_viewportsOrder.clear();
        m_viewports.clear();
    }

    auto ViewportManager::begin() noexcept -> Iterator
    {
        return Iterator{m_viewportsOrder.begin(), *this};
    }

    auto ViewportManager::begin() const noexcept -> ConstIterator
    {
        return ConstIterator{m_viewportsOrder.begin(), *this};
    }

    auto ViewportManager::end() noexcept -> Iterator
    {
        return Iterator{m_viewportsOrder.end(), *this};
    }

    auto ViewportManager::end() const noexcept -> ConstIterator
    {
        return ConstIterator{m_viewportsOrder.end(), *this};
    }

    ViewportManager::Iterator::Iterator(const KeyIter aKeyIter, ViewportManager& aManager) noexcept
        : m_keyIter{aKeyIter}, m_manager{&aManager}
    {
    }

    auto ViewportManager::Iterator::operator*() const noexcept -> reference
    {
        const auto key = *m_keyIter;
        const auto iter = m_manager->m_viewports.find(key);
        return iter.operator*();
    }

    auto ViewportManager::Iterator::operator->() const noexcept -> pointer
    {
        const auto key = *m_keyIter;
        const auto iter = m_manager->m_viewports.find(key);
        return iter.operator->();
    }

    auto ViewportManager::Iterator::operator++() noexcept -> Iterator&
    {
        ++m_keyIter;
        return *this;
    }

    auto ViewportManager::Iterator::operator++(int) noexcept -> Iterator
    {
        const auto temp = *this;
        ++m_keyIter;
        return temp;
    }

    auto ViewportManager::Iterator::operator--() noexcept -> Iterator&
    {
        --m_keyIter;
        return *this;
    }

    auto ViewportManager::Iterator::operator--(int) noexcept -> Iterator
    {
        const auto temp = *this;
        --m_keyIter;
        return temp;
    }

    ViewportManager::ConstIterator::ConstIterator(const KeyIter aKeyIter, const ViewportManager& aManager) noexcept
        : m_keyIter{aKeyIter}, m_manager{&aManager}
    {
    }

    auto ViewportManager::ConstIterator::operator*() const noexcept -> reference
    {
        const auto key = *m_keyIter;
        const auto iter = m_manager->m_viewports.find(key);
        return iter.operator*();
    }

    auto ViewportManager::ConstIterator::operator->() const noexcept -> pointer
    {
        const auto key = *m_keyIter;
        const auto iter = m_manager->m_viewports.find(key);
        return iter.operator->();
    }

    auto ViewportManager::ConstIterator::operator++() noexcept -> ConstIterator&
    {
        ++m_keyIter;
        return *this;
    }

    auto ViewportManager::ConstIterator::operator++(int) noexcept -> ConstIterator
    {
        const auto temp = *this;
        ++m_keyIter;
        return temp;
    }

    auto ViewportManager::ConstIterator::operator--() noexcept -> ConstIterator&
    {
        --m_keyIter;
        return *this;
    }

    auto ViewportManager::ConstIterator::operator--(int) noexcept -> ConstIterator
    {
        const auto temp = *this;
        --m_keyIter;
        return temp;
    }
}
