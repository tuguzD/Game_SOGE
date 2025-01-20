#ifndef SOGE_GRAPHICS_GRAPHICSRESOURCEMANAGER_HPP
#define SOGE_GRAPHICS_GRAPHICSRESOURCEMANAGER_HPP

#include "SOGE/Graphics/GraphicsResource.hpp"
#include "SOGE/System/Memory.hpp"

#include <EASTL/span.h>


namespace soge
{
    class GraphicsResourceManager
    {
    private:
        using Key = UUIDv4::UUID;
        using UniqueResource = UniquePtr<GraphicsResource>;

        using Resources = eastl::hash_map<Key, UniqueResource>;
        Resources m_resources;

        using ResourceRefs = eastl::vector<eastl::reference_wrapper<GraphicsResource>>;
        ResourceRefs m_resourceRefs;

    public:
        template <typename T, typename... Args>
        [[nodiscard]]
        eastl::pair<T&, Key> CreateResource(Args&&... args);

        [[nodiscard]]
        eastl::pair<GraphicsResource&, Key> CreateResource(UniqueResource aResource);

        [[nodiscard]]
        GraphicsResource* GetResource(const Key& aKey) const;

        UniqueResource DestroyResource(const Key& aKey);

        void Clear();

        using ResourcesSpan = eastl::span<eastl::reference_wrapper<GraphicsResource>>;
        [[nodiscard]]
        ResourcesSpan GetResources() noexcept;
    };

    template <typename T, typename... Args>
    auto GraphicsResourceManager::CreateResource(Args&&... args) -> eastl::pair<T&, Key>
    {
        UniquePtr<T> resource = CreateUnique<T>(std::forward<Args>(args)...);
        T* resourcePtr = resource.get();

        const auto [_, key] = CreateResource(std::move(resource));
        return {*resourcePtr, key};
    }
}

#endif // SOGE_GRAPHICS_GRAPHICSRESOURCEMANAGER_HPP
