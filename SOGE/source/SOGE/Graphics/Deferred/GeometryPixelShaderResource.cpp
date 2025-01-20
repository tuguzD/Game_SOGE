#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/GeometryPixelShaderResource.hpp"


namespace
{
    constexpr auto g_shaderName = "SOGE geometry pipeline pixel shader";
    constexpr auto g_shaderPath = "./resources/shaders/deferred_geometry.hlsl";

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
    GeometryPixelShaderResource::GeometryPixelShaderResource(GraphicsCore& aCore)
        : ShaderResource{aCore, g_shaderName, g_shaderPath, CreateShaderDesc()}
    {
    }
}
