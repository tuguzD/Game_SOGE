#ifndef SOGE_GRAPHICS_VIEWPORTMANAGER_HPP
#define SOGE_GRAPHICS_VIEWPORTMANAGER_HPP

#include "SOGE/Utils/UUID.hpp"

#include <EASTL/hash_map.h>
#include <EASTL/list.h>
#include <EASTL/optional.h>
#include <nvrhi/nvrhi.h>

#include <ranges>


namespace soge
{
    class ViewportManager
    {
    public:
        using CameraId = UUIDv4::UUID;

        struct Viewport
        {
            nvrhi::Viewport m_viewport;
            CameraId m_cameraId;
        };

    private:
        using Key = UUIDv4::UUID;

        using Viewports = eastl::hash_map<Key, Viewport>;
        Viewports m_viewports;

        using ViewportsOrder = eastl::list<Key>;
        ViewportsOrder m_viewportsOrder;

    public:
        class Iterator;
        class ConstIterator;

        [[nodiscard]]
        eastl::pair<Viewport&, Key> CreateViewport(const Viewport& aViewport);

        [[nodiscard]]
        Viewport* GetViewport(const Key& aKey);

        eastl::optional<Viewport> DestroyViewport(const Key& aKey);

        void Clear();

        // NOLINTBEGIN(readability-identifier-naming)
        Iterator begin() noexcept;
        ConstIterator begin() const noexcept;

        Iterator end() noexcept;
        ConstIterator end() const noexcept;
        // NOLINTEND(readability-identifier-naming)
    };

    class ViewportManager::Iterator
    {
    private:
        friend ViewportManager;
        using KeyIter = ViewportsOrder::iterator;

        explicit Iterator(KeyIter aKeyIter, ViewportManager& aManager) noexcept;

        KeyIter m_keyIter;
        ViewportManager* m_manager;

    public:
        // NOLINTBEGIN(readability-identifier-naming)
        using difference_type = KeyIter::difference_type;
        using value_type = Viewports::iterator::value_type;
        using pointer = Viewports::iterator::pointer;
        using reference = Viewports::iterator::reference;
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

    class ViewportManager::ConstIterator
    {
    private:
        friend ViewportManager;
        using KeyIter = ViewportsOrder::const_iterator;

        explicit ConstIterator(KeyIter aKeyIter, const ViewportManager& aManager) noexcept;

        KeyIter m_keyIter;
        const ViewportManager* m_manager;

    public:
        // NOLINTBEGIN(readability-identifier-naming)
        using difference_type = KeyIter::difference_type;
        using value_type = Viewports::const_iterator::value_type;
        using pointer = Viewports::const_iterator::pointer;
        using reference = Viewports::const_iterator::reference;
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


    static_assert(std::bidirectional_iterator<ViewportManager::Iterator>);
    static_assert(std::bidirectional_iterator<ViewportManager::ConstIterator>);

    static_assert(std::ranges::bidirectional_range<ViewportManager>);
}

#endif // SOGE_GRAPHICS_VIEWPORTMANAGER_HPP
