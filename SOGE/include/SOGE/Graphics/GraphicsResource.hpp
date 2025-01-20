#ifndef SOGE_GRAPHICS_GRAPHICSRESOURCE_HPP
#define SOGE_GRAPHICS_GRAPHICSRESOURCE_HPP

#include "SOGE/Content/ResourceBase.hpp"

#include <nvrhi/nvrhi.h>


namespace soge
{
    class GraphicsResource : public ResourceBase
    {
    public:
        GraphicsResource(eastl::string_view aName, cppfs::FilePath aFullPath);

        [[nodiscard]]
        constexpr virtual nvrhi::IResource* GetResource() = 0;

        constexpr virtual void WriteResource(nvrhi::ICommandList& aCommandList) = 0;
    };
}

#endif // SOGE_GRAPHICS_GRAPHICSRESOURCE_HPP
