#include "sogepch.hpp"

#include "SOGE/Content/ResourceBase.hpp"

#include "SOGE/Utils/StringHelpers.hpp"


namespace soge
{
    ResourceBase::ResourceBase(const eastl::string_view aName, cppfs::FilePath aFullPath)
        : m_fullPath(std::move(aFullPath)), m_name(aName), m_loaded(false)
    {
        m_resourceUUID = UUID::Generate();
    }

    void ResourceBase::SetFilePath(const eastl::string& aFullPath)
    {
        m_fullPath.setPath(EANarrowToStd(aFullPath));
    }

    void ResourceBase::SetFilePath(const cppfs::FilePath& aFullPath)
    {
        m_fullPath.setPath(aFullPath.fullPath());
    }

    void ResourceBase::SetLoaded(const bool aLoaded)
    {
        m_loaded = aLoaded;
    }

    eastl::string_view ResourceBase::GetName() const
    {
        return m_name;
    }

    eastl::string_view ResourceBase::GetFullPath() const
    {
        const auto& fullPath = m_fullPath.fullPath();
        return {fullPath.c_str(), fullPath.size()};
    }

    bool ResourceBase::IsLoaded() const
    {
        return m_loaded;
    }

    UUIDv4::UUID ResourceBase::GetUUID() const
    {
        return m_resourceUUID;
    }
}
