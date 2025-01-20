#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/PointLightPixelShaderResource.hpp"


namespace
{
    constexpr auto g_shaderName = "SOGE point light pipeline pixel shader";
    constexpr auto g_shaderPath = "./resources/shaders/deferred_point_light.hlsl";

    [[nodiscard]]
    nvrhi::ShaderDesc CreateShaderDesc()
    {
        nvrhi::ShaderDesc shaderDesc{};
        shaderDesc.shaderType = nvrhi::ShaderType::Pixel;
        shaderDesc.debugName = g_shaderName;
        shaderDesc.entryName = "PSMain";

        return shaderDesc;
    }
}

namespace soge
{
    PointLightPixelShaderResource::PointLightPixelShaderResource(GraphicsCore& aCore)
        : ShaderResource{aCore, g_shaderName, g_shaderPath, CreateShaderDesc()}
    {
    }
}
