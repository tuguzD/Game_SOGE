#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/DirectionalLightPixelShaderResource.hpp"


namespace
{
    constexpr auto g_shaderName = "SOGE directional light pipeline pixel shader";
    constexpr auto g_shaderPath = "./resources/shaders/deferred_directional_light.hlsl";

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
    DirectionalLightPixelShaderResource::DirectionalLightPixelShaderResource(GraphicsCore& aCore)
        : ShaderResource{aCore, g_shaderName, g_shaderPath, CreateShaderDesc()}
    {
    }
}
