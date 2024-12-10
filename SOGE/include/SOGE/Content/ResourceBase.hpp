#ifndef SOGE_CONTENT_RESOURCEBASE_HPP
#define SOGE_CONTENT_RESOURCEBASE_HPP

#include "SOGE/Utils/UUID.hpp"
#include <cppfs/FilePath.h>



namespace soge
{
    class ResourceBase
    {
    private:
        UUIDv4::UUID m_resourceUUID;
        cppfs::FilePath m_fullPath;

    protected:
        eastl::string m_name;
        bool m_loaded;

        void SetFilePath(const eastl::string& aFullPath);
        void SetFilePath(const cppfs::FilePath& aFullPath);
        void SetLoaded(bool aLoaded);

    public:
        ResourceBase(const eastl::string& aName, const cppfs::FilePath& aFullPath);
        ~ResourceBase();

        // True if reload was successful
        virtual bool Reload() = 0;

        // Free most of the memory but save info to load agean
        virtual void Unload() = 0;

        // Free all memory
        virtual void Destroy() = 0;

        const eastl::string& GetName() const;
        const eastl::string& GetFullPath() const;

        bool IsLoaded() const;
        UUIDv4::UUID GetUUID() const;

    };
}

#endif // !SOGE_CONTENT_RESOURCEBASE_HPP
