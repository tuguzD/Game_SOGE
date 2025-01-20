#include "sogepch.hpp"

#include "SOGE/Graphics/GraphicsResource.hpp"


namespace soge
{
    GraphicsResource::GraphicsResource(const eastl::string_view aName, cppfs::FilePath aFullPath)
        : ResourceBase(aName, std::move(aFullPath))
    {
    }
}
