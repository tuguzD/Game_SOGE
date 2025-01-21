#ifndef SOGE_GRAPHICS_RESOURCES_SHADERRESOURCE_HPP
#define SOGE_GRAPHICS_RESOURCES_SHADERRESOURCE_HPP

#include "SOGE/Content/ResourceBase.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class ShaderResource : public GraphicsEntity, public ResourceBase
    {
    private:
        void Initialize();

        eastl::reference_wrapper<GraphicsCore> m_core;

        nvrhi::ShaderHandle m_shaderHandle;
        nvrhi::ShaderDesc m_shaderDesc;

    public:
        ShaderResource(GraphicsCore& aCore, eastl::string_view aName, cppfs::FilePath aFullPath,
                       nvrhi::ShaderDesc aShaderDesc);

        [[nodiscard]]
        nvrhi::IShader* GetShaderResource();

        void WriteResources(nvrhi::ICommandList& aCommandList) override;

        bool Reload() override;
        void Unload() override;
        void Destroy() override;
    };
}

#endif // SOGE_GRAPHICS_RESOURCES_SHADERRESOURCE_HPP
