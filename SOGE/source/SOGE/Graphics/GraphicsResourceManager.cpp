#include "sogepch.hpp"

#include "SOGE/Graphics/GraphicsResourceManager.hpp"


namespace soge
{
    auto GraphicsResourceManager::CreateResource(UniqueResource aResource) -> eastl::pair<GraphicsResource&, Key>
    {
        const auto key = aResource->GetUUID();

        const auto [iter, created] = m_resources.try_emplace(key, std::move(aResource));
        if (created)
        {
            m_resourceRefs.push_back(*iter->second);
        }
        iter->second->Reload();

        return {*iter->second, key};
    }

    GraphicsResource* GraphicsResourceManager::GetResource(const Key& aKey) const
    {
        const auto iter = m_resources.find(aKey);
        if (iter == m_resources.end())
        {
            return nullptr;
        }

        return iter->second.get();
    }

    auto GraphicsResourceManager::DestroyResource(const Key& aKey) -> UniqueResource
    {
        const auto iter = m_resources.find(aKey);
        if (iter == m_resources.end())
        {
            return UniqueResource{};
        }

        UniqueResource resource = std::move(iter->second);
        m_resources.erase(iter);

        auto predicate = [&resource](auto&& aItem) { return resource.get() == &aItem.get(); };
        m_resourceRefs.erase(eastl::remove_if(m_resourceRefs.begin(), m_resourceRefs.end(), predicate),
                             m_resourceRefs.end());

        resource->Unload();
        return resource;
    }

    void GraphicsResourceManager::Clear()
    {
        m_resources.clear();
    }

    auto GraphicsResourceManager::GetResources() noexcept -> ResourcesSpan
    {
        return m_resourceRefs;
    }
}
