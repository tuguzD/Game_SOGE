#ifndef SOGE_CONTENT_RESOURCEACCESSOR_HPP
#define SOGE_CONTENT_RESOURCEACCESSOR_HPP


namespace soge
{
    /**
     * @class ResourceAccessorBase ResourceBase.hpp "SOGE/Content/ResourceBase.hpp"
     * @brief Resource implementation specific details.
     * @details Some systems can access resource but can't access
     * protected members. To solve this problem, we contain shared
     * resources in this structure.
     */
    template <typename ResourcePtr>
    class ResourceAccessor
    {
    private:
        bool m_isValid;

    protected:
        ResourcePtr* m_resourcePtr = nullptr;

    protected:
        ResourceAccessor() = default;
        ~ResourceAccessor() = default;

        void MakeAccessible(ResourcePtr* aResource);
        void ReleaseAccess();
        bool IsValid() const;
    };

    template <typename ResourcePtr>
    inline void ResourceAccessor<ResourcePtr>::MakeAccessible(ResourcePtr* aResource)
    {
        if (!IsValid())
            m_resourcePtr = aResource;
    }

    template <typename ResourcePtr>
    inline void ResourceAccessor<ResourcePtr>::ReleaseAccess()
    {
        m_resourcePtr = nullptr;
    }

    template <typename ResourcePtr>
    inline bool ResourceAccessor<ResourcePtr>::IsValid() const
    {
        return m_resourcePtr != nullptr;
    }
}

#endif // SOGE_CONTENT_RESOURCEACCESSOR_HPP
