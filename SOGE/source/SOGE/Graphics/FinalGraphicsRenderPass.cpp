#include "sogepch.hpp"

#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"

#include "SOGE/Graphics/GraphicsSwapchain.hpp"
#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"

#include <nvrhi/utils.h>


namespace soge
{
    FinalGraphicsRenderPass::FinalGraphicsRenderPass(GraphicsCore& aCore) : m_core{aCore}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        const auto swapChainTextures = aCore.GetSwapchain()->GetTextures();
        assert(!swapChainTextures.empty());
        const auto& swapChainTextureDesc = swapChainTextures[0].get().getDesc();

        SOGE_INFO_LOG("Creating NVRHI depth texture for final render pass...");
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
        nvrhiDepthTextureDesc.debugName = "SOGE final render pass depth texture";

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

        m_nvrhiFramebuffers.reserve(swapChainTextures.size());
        for (std::size_t index = 0; index < swapChainTextures.size(); index++)
        {
            nvrhi::ITexture* nvrhiColorTexture = &swapChainTextures[index].get();

            SOGE_INFO_LOG("Creating NVRHI framebuffer (frame {}) for final render pass...", index);
            nvrhi::FramebufferDesc framebufferDesc{};
            framebufferDesc.addColorAttachment(nvrhiColorTexture);
            framebufferDesc.setDepthAttachment(nvrhiDepthTexture);

            nvrhi::FramebufferHandle nvrhiFramebuffer = device.createFramebuffer(framebufferDesc);
            m_nvrhiFramebuffers.push_back(nvrhiFramebuffer);
        }
    }

    nvrhi::IFramebuffer& FinalGraphicsRenderPass::GetFramebuffer()
    {
        const auto currentFrameIndex = m_core.get().GetSwapchain()->GetCurrentTextureIndex();
        return *m_nvrhiFramebuffers[currentFrameIndex];
    }

    void FinalGraphicsRenderPass::ClearFramebuffer(nvrhi::ICommandList& aCommandList)
    {
        nvrhi::IFramebuffer& currentFramebuffer = GetFramebuffer();
        const nvrhi::FramebufferDesc& framebufferDesc = currentFramebuffer.getDesc();
        for (std::uint32_t index = 0; index < framebufferDesc.colorAttachments.size(); index++)
        {
            nvrhi::utils::ClearColorAttachment(&aCommandList, &currentFramebuffer, index, nvrhi::Color{});
        }
        nvrhi::utils::ClearDepthStencilAttachment(&aCommandList, &currentFramebuffer, 1.0f, 0);
    }
}
