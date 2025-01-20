#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/GeometryVertexShaderResource.hpp"


namespace
{
    constexpr auto g_shaderName = "SOGE geometry pipeline vertex shader";
    constexpr auto g_shaderPath = "./resources/shaders/deferred_geometry.hlsl";

    [[nodiscard]]
    nvrhi::ShaderDesc CreateShaderDesc()
    {
        nvrhi::ShaderDesc shaderDesc{};
        shaderDesc.shaderType = nvrhi::ShaderType::Vertex;
        shaderDesc.debugName = g_shaderName;
        shaderDesc.entryName = "VSMain";

        return shaderDesc;
    }
}

namespace soge
{
    GeometryVertexShaderResource::GeometryVertexShaderResource(GraphicsCore& aCore)
        : ShaderResource{aCore, g_shaderName, g_shaderPath, CreateShaderDesc()}
    {
    }
}
