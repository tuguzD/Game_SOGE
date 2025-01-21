#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/PointLightVertexShaderResource.hpp"


namespace
{
    constexpr auto g_shaderName = "SOGE point light pipeline vertex shader";
    constexpr auto g_shaderPath = "./resources/shaders/deferred_point_light.hlsl";

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
    PointLightVertexShaderResource::PointLightVertexShaderResource(GraphicsCore& aCore)
        : ShaderResource{aCore, g_shaderName, g_shaderPath, CreateShaderDesc()}
    {
    }
}
