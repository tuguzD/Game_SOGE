#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/GeometryGraphicsRenderPass.hpp"

#include "SOGE/Graphics/GraphicsSwapchain.hpp"
#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"

#include <nvrhi/utils.h>


namespace soge
{
    GeometryGraphicsRenderPass::GeometryGraphicsRenderPass(GraphicsCore& aCore) : m_core{aCore}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        const auto swapChainTextures = aCore.GetSwapchain()->GetTextures();
        assert(!swapChainTextures.empty());
        const auto& swapChainTextureDesc = swapChainTextures[0].get().getDesc();

        SOGE_INFO_LOG("Creating NVRHI albedo texture for geometry render pass...");
        nvrhi::TextureDesc textureDesc{};
        textureDesc.dimension = nvrhi::TextureDimension::Texture2D;
        textureDesc.format = nvrhi::Format::RGBA16_FLOAT;
        textureDesc.width = swapChainTextureDesc.width;
        textureDesc.height = swapChainTextureDesc.height;
        textureDesc.isRenderTarget = true;
        textureDesc.isShaderResource = true;
        textureDesc.mipLevels = 1;
        textureDesc.useClearValue = true;
        textureDesc.clearValue = nvrhi::Color{};
        textureDesc.initialState = nvrhi::ResourceStates::RenderTarget;
        textureDesc.keepInitialState = true;
        textureDesc.debugName = "SOGE geometry render pass albedo texture";
        m_nvrhiAlbedoTexture = device.createTexture(textureDesc);

        SOGE_INFO_LOG("Creating NVRHI normal texture for geometry render pass...");
        textureDesc.debugName = "SOGE geometry render pass normal texture";
        m_nvrhiNormalTexture = device.createTexture(textureDesc);

        SOGE_INFO_LOG("Creating NVRHI ambient texture for geometry render pass...");
        textureDesc.debugName = "SOGE geometry render pass ambient texture";
        m_nvrhiAmbientTexture = device.createTexture(textureDesc);

        SOGE_INFO_LOG("Creating NVRHI diffuse texture for geometry render pass...");
        textureDesc.debugName = "SOGE geometry render pass diffuse texture";
        m_nvrhiDiffuseTexture = device.createTexture(textureDesc);

        SOGE_INFO_LOG("Creating NVRHI specular shininess texture for geometry render pass...");
        textureDesc.debugName = "SOGE geometry render pass specular shininess texture";
        m_nvrhiSpecularShininessTexture = device.createTexture(textureDesc);

        SOGE_INFO_LOG("Creating NVRHI emissive texture for geometry render pass...");
        textureDesc.debugName = "SOGE geometry render pass emissive texture";
        m_nvrhiEmissiveTexture = device.createTexture(textureDesc);

        SOGE_INFO_LOG("Creating NVRHI depth texture for geometry render pass...");
        textureDesc.isTypeless = true;
        textureDesc.clearValue = nvrhi::Color{1.0f, 0.0f, 0.0f, 0.0f};
        textureDesc.initialState = nvrhi::ResourceStates::DepthWrite;
        textureDesc.debugName = "SOGE geometry render pass depth texture";

        const nvrhi::FormatSupport requiredDepthFeatures =
            nvrhi::FormatSupport::Texture | nvrhi::FormatSupport::DepthStencil | nvrhi::FormatSupport::ShaderLoad;
        constexpr std::array requestedDepthFormats{
            nvrhi::Format::D24S8,
            nvrhi::Format::D32S8,
            nvrhi::Format::D32,
            nvrhi::Format::D16,
        };
        textureDesc.format = nvrhi::utils::ChooseFormat(&device, requiredDepthFeatures, requestedDepthFormats.data(),
                                                        requestedDepthFormats.size());

        const nvrhi::TextureHandle nvrhiDepthTexture = device.createTexture(textureDesc);

        SOGE_INFO_LOG("Creating NVRHI framebuffer for geometry render pass...");
        nvrhi::FramebufferDesc framebufferDesc{};
        framebufferDesc.addColorAttachment(m_nvrhiAlbedoTexture);
        framebufferDesc.addColorAttachment(m_nvrhiNormalTexture);
        framebufferDesc.addColorAttachment(m_nvrhiAmbientTexture);
        framebufferDesc.addColorAttachment(m_nvrhiDiffuseTexture);
        framebufferDesc.addColorAttachment(m_nvrhiSpecularShininessTexture);
        framebufferDesc.addColorAttachment(m_nvrhiEmissiveTexture);
        framebufferDesc.setDepthAttachment(nvrhiDepthTexture);

        m_nvrhiFramebuffer = device.createFramebuffer(framebufferDesc);
    }

    void GeometryGraphicsRenderPass::ClearFramebuffer(nvrhi::ICommandList& aCommandList)
    {
        nvrhi::IFramebuffer& currentFramebuffer = GetFramebuffer();

        nvrhi::utils::ClearColorAttachment(&aCommandList, &currentFramebuffer, 0, nvrhi::Color{});
        nvrhi::utils::ClearColorAttachment(&aCommandList, &currentFramebuffer, 1, nvrhi::Color{});
        nvrhi::utils::ClearColorAttachment(&aCommandList, &currentFramebuffer, 2, nvrhi::Color{});
        nvrhi::utils::ClearColorAttachment(&aCommandList, &currentFramebuffer, 3, nvrhi::Color{});
        nvrhi::utils::ClearColorAttachment(&aCommandList, &currentFramebuffer, 4, nvrhi::Color{});
        nvrhi::utils::ClearColorAttachment(&aCommandList, &currentFramebuffer, 5, nvrhi::Color{});

        nvrhi::utils::ClearDepthStencilAttachment(&aCommandList, &currentFramebuffer, 1.0f, 0);
    }

    nvrhi::ITexture& GeometryGraphicsRenderPass::GetAlbedoTexture()
    {
        return *m_nvrhiAlbedoTexture;
    }

    nvrhi::ITexture& GeometryGraphicsRenderPass::GetNormalTexture()
    {
        return *m_nvrhiNormalTexture;
    }

    nvrhi::ITexture& GeometryGraphicsRenderPass::GetAmbientTexture()
    {
        return *m_nvrhiAmbientTexture;
    }

    nvrhi::ITexture& GeometryGraphicsRenderPass::GetDiffuseTexture()
    {
        return *m_nvrhiDiffuseTexture;
    }

    nvrhi::ITexture& GeometryGraphicsRenderPass::GetSpecularShininessTexture()
    {
        return *m_nvrhiSpecularShininessTexture;
    }

    nvrhi::ITexture& GeometryGraphicsRenderPass::GetEmissiveTexture()
    {
        return *m_nvrhiEmissiveTexture;
    }

    nvrhi::IFramebuffer& GeometryGraphicsRenderPass::GetFramebuffer()
    {
        return *m_nvrhiFramebuffer;
    }
}
