#ifndef SOGE_GRAPHICS_RESOURCES_SHADERRESOURCE_HPP
#define SOGE_GRAPHICS_RESOURCES_SHADERRESOURCE_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/GraphicsResource.hpp"


namespace soge
{
    class ShaderResource : public GraphicsResource
    {
    private:
        nvrhi::ShaderHandle m_shaderHandle;

    public:
        ShaderResource(GraphicsCore& aCore, eastl::string_view aName, const cppfs::FilePath& aFullPath,
                       const nvrhi::ShaderDesc& aShaderDesc);

        [[nodiscard]]
        nvrhi::IShader& GetResource() override;
    };
}

#endif // SOGE_GRAPHICS_RESOURCES_SHADERRESOURCE_HPP
