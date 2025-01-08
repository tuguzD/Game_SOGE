#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsRenderPass.hpp"

#include "SOGE/Graphics/GraphicsSwapchain.hpp"
#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"

#include <nvrhi/utils.h>


namespace soge
{
    D3D12GraphicsRenderPass::D3D12GraphicsRenderPass(D3D12GraphicsCore& aCore) : m_core{aCore}
    {
        const auto swapChainTextures = aCore.m_swapChain->GetTextures();
        assert(!swapChainTextures.empty());
        const auto& swapChainTextureDesc = swapChainTextures[0].get().getDesc();

        SOGE_INFO_LOG("Creating NVRHI depth texture...");
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
        nvrhiDepthTextureDesc.debugName = "SOGE depth texture";

        const nvrhi::FormatSupport requiredDepthFeatures =
            nvrhi::FormatSupport::Texture | nvrhi::FormatSupport::DepthStencil | nvrhi::FormatSupport::ShaderLoad;
        constexpr std::array requestedDepthFormats{
            nvrhi::Format::D24S8,
            nvrhi::Format::D32S8,
            nvrhi::Format::D32,
            nvrhi::Format::D16,
        };
        nvrhiDepthTextureDesc.format = nvrhi::utils::ChooseFormat(
            aCore.m_nvrhiDevice, requiredDepthFeatures, requestedDepthFormats.data(), requestedDepthFormats.size());

        const nvrhi::TextureHandle nvrhiDepthTexture = aCore.m_nvrhiDevice->createTexture(nvrhiDepthTextureDesc);

        m_nvrhiFramebuffers.reserve(swapChainTextures.size());
        for (std::size_t index = 0; index < swapChainTextures.size(); index++)
        {
            nvrhi::ITexture* nvrhiColorTexture = &swapChainTextures[index].get();

            SOGE_INFO_LOG("Creating NVRHI framebuffer (frame {})...", index);
            nvrhi::FramebufferDesc framebufferDesc{};
            framebufferDesc.addColorAttachment(nvrhiColorTexture);
            framebufferDesc.setDepthAttachment(nvrhiDepthTexture);

            nvrhi::FramebufferHandle nvrhiFramebuffer = aCore.m_nvrhiDevice->createFramebuffer(framebufferDesc);
            m_nvrhiFramebuffers.push_back(nvrhiFramebuffer);
        }
    }

    D3D12GraphicsRenderPass::D3D12GraphicsRenderPass(D3D12GraphicsRenderPass&& aOther) noexcept : m_core{aOther.m_core}
    {
        swap(aOther);
    }

    D3D12GraphicsRenderPass& D3D12GraphicsRenderPass::operator=(D3D12GraphicsRenderPass&& aOther) noexcept
    {
        swap(aOther);
        return *this;
    }

    void D3D12GraphicsRenderPass::swap(D3D12GraphicsRenderPass& aOther) noexcept
    {
        using std::swap;

        eastl::swap(m_core, aOther.m_core);
        swap(m_nvrhiFramebuffers, aOther.m_nvrhiFramebuffers);
    }

    D3D12GraphicsRenderPass::~D3D12GraphicsRenderPass()
    {
        if (!m_nvrhiFramebuffers.empty())
        {
            SOGE_INFO_LOG("Destroying NVRHI framebuffers...");
            m_nvrhiFramebuffers.clear();
        }
    }

    auto D3D12GraphicsRenderPass::GetFramebuffer() -> FramebufferRef
    {
        const auto currentFrameIndex = m_core.get().m_swapChain->GetCurrentTextureIndex();
        return *m_nvrhiFramebuffers[currentFrameIndex];
    }
}
