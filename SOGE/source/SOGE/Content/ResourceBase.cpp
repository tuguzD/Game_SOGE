#include "sogepch.hpp"
#include "SOGE/Content/ResourceBase.hpp"
#include "SOGE/Utils/StringHelpers.hpp"


namespace soge
{
    ResourceBase::ResourceBase(const eastl::string& aName, const cppfs::FilePath& aFullPath)
        : m_name(aName), m_fullPath(aFullPath)
    {
        m_resourceUUID = UUID::Generate();
    }

    ResourceBase::~ResourceBase()
    {
    }

    void ResourceBase::SetFilePath(const eastl::string& aFullPath)
    {
        m_fullPath.setPath(EANarrowToStd(aFullPath));
    }

    void ResourceBase::SetFilePath(const cppfs::FilePath& aFullPath)
    {
        m_fullPath.setPath(aFullPath.fullPath());
    }

    const eastl::string& ResourceBase::GetName() const
    {
        return m_name;
    }

    const eastl::string& ResourceBase::GetFullPath() const
    {
        return m_fullPath.fullPath().c_str();
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
