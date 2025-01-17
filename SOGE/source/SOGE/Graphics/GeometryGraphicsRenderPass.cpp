#include "sogepch.hpp"

#include "SOGE/Graphics/GeometryGraphicsRenderPass.hpp"

#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"
#include "SOGE/Graphics/GraphicsSwapchain.hpp"

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
        nvrhi::TextureDesc nvrhiAlbedoTextureDesc{};
        nvrhiAlbedoTextureDesc.dimension = nvrhi::TextureDimension::Texture2D;
        nvrhiAlbedoTextureDesc.format = nvrhi::Format::RGBA16_FLOAT;
        nvrhiAlbedoTextureDesc.width = swapChainTextureDesc.width;
        nvrhiAlbedoTextureDesc.height = swapChainTextureDesc.height;
        nvrhiAlbedoTextureDesc.isRenderTarget = true;
        nvrhiAlbedoTextureDesc.isShaderResource = true;
        nvrhiAlbedoTextureDesc.mipLevels = 1;
        nvrhiAlbedoTextureDesc.useClearValue = true;
        nvrhiAlbedoTextureDesc.clearValue = nvrhi::Color{};
        nvrhiAlbedoTextureDesc.initialState = nvrhi::ResourceStates::ShaderResource;
        nvrhiAlbedoTextureDesc.keepInitialState = true;
        nvrhiAlbedoTextureDesc.debugName = "SOGE geometry render pass albedo texture";

        m_nvrhiAlbedoTexture = device.createTexture(nvrhiAlbedoTextureDesc);

        SOGE_INFO_LOG("Creating NVRHI depth texture for geometry render pass...");
        nvrhi::TextureDesc nvrhiDepthTextureDesc{};
        nvrhiDepthTextureDesc.dimension = nvrhi::TextureDimension::Texture2D;
        nvrhiDepthTextureDesc.width = swapChainTextureDesc.width;
        nvrhiDepthTextureDesc.height = swapChainTextureDesc.height;
        nvrhiDepthTextureDesc.isRenderTarget = true;
        nvrhiDepthTextureDesc.isShaderResource = true;
        nvrhiDepthTextureDesc.isTypeless = true;
        nvrhiDepthTextureDesc.mipLevels = 1;
        nvrhiDepthTextureDesc.useClearValue = true;
        nvrhiDepthTextureDesc.clearValue = nvrhi::Color{1.0f, 0.0f, 0.0f, 0.0f};
        nvrhiDepthTextureDesc.initialState = nvrhi::ResourceStates::DepthWrite;
        nvrhiDepthTextureDesc.keepInitialState = true;
        nvrhiDepthTextureDesc.debugName = "SOGE geometry render pass depth texture";

        const nvrhi::FormatSupport requiredDepthFeatures =
            nvrhi::FormatSupport::Texture | nvrhi::FormatSupport::DepthStencil | nvrhi::FormatSupport::ShaderLoad;
        constexpr std::array requestedDepthFormats{
            nvrhi::Format::D24S8,
            nvrhi::Format::D32S8,
            nvrhi::Format::D32,
            nvrhi::Format::D16,
        };
        nvrhiDepthTextureDesc.format = nvrhi::utils::ChooseFormat(
            &device, requiredDepthFeatures, requestedDepthFormats.data(), requestedDepthFormats.size());

        const nvrhi::TextureHandle nvrhiDepthTexture = device.createTexture(nvrhiDepthTextureDesc);

        SOGE_INFO_LOG("Creating NVRHI framebuffer for geometry render pass...");
        nvrhi::FramebufferDesc framebufferDesc{};
        framebufferDesc.addColorAttachment(m_nvrhiAlbedoTexture);
        framebufferDesc.setDepthAttachment(nvrhiDepthTexture);

        m_nvrhiFramebuffer = device.createFramebuffer(framebufferDesc);
    }

    GeometryGraphicsRenderPass::GeometryGraphicsRenderPass(GeometryGraphicsRenderPass&& aOther) noexcept
        : m_core{aOther.m_core}
    {
        swap(aOther);
    }

    GeometryGraphicsRenderPass& GeometryGraphicsRenderPass::operator=(GeometryGraphicsRenderPass&& aOther) noexcept
    {
        swap(aOther);
        return *this;
    }

    GeometryGraphicsRenderPass::~GeometryGraphicsRenderPass()
    {
        SOGE_INFO_LOG("Destroying NVRHI framebuffer of geometry render pass...");
        m_nvrhiFramebuffer = nullptr;

        SOGE_INFO_LOG("Destroying NVRHI albedo texture of geometry render pass...");
        m_nvrhiAlbedoTexture = nullptr;
    }

    void GeometryGraphicsRenderPass::swap(GeometryGraphicsRenderPass& aOther) noexcept
    {
        using std::swap;

        eastl::swap(m_core, aOther.m_core);

        swap(m_nvrhiAlbedoTexture, aOther.m_nvrhiAlbedoTexture);
        swap(m_nvrhiFramebuffer, aOther.m_nvrhiFramebuffer);
    }

    void GeometryGraphicsRenderPass::ClearFramebuffer(nvrhi::ICommandList& aCommandList)
    {
        nvrhi::IFramebuffer& currentFramebuffer = GetFramebuffer();

        // Clear albedo texture
        nvrhi::utils::ClearColorAttachment(&aCommandList, &currentFramebuffer, 0, nvrhi::Color{});

        nvrhi::utils::ClearDepthStencilAttachment(&aCommandList, &currentFramebuffer, 1.0f, 0);
    }

    nvrhi::ITexture& GeometryGraphicsRenderPass::GetAlbedoTexture()
    {
        return *m_nvrhiAlbedoTexture;
    }

    nvrhi::IFramebuffer& GeometryGraphicsRenderPass::GetFramebuffer()
    {
        return *m_nvrhiFramebuffer;
    }
}
