#ifndef SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSRENDERPASS_HPP
#define SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSRENDERPASS_HPP

#include "SOGE/Graphics/GraphicsRenderPass.hpp"

#include <EASTL/vector.h>


namespace soge
{
    class D3D12GraphicsCore;

    class D3D12GraphicsRenderPass : public GraphicsRenderPass
    {
    private:
        eastl::reference_wrapper<D3D12GraphicsCore> m_core;

        eastl::vector<nvrhi::FramebufferHandle> m_nvrhiFramebuffers;

    public:
        explicit D3D12GraphicsRenderPass(D3D12GraphicsCore& aCore);

        D3D12GraphicsRenderPass(const D3D12GraphicsRenderPass&) = delete;
        D3D12GraphicsRenderPass& operator=(const D3D12GraphicsRenderPass&) = delete;

        D3D12GraphicsRenderPass(D3D12GraphicsRenderPass&& aOther) noexcept;
        D3D12GraphicsRenderPass& operator=(D3D12GraphicsRenderPass&& aOther) noexcept;

        ~D3D12GraphicsRenderPass() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(D3D12GraphicsRenderPass& aOther) noexcept;

        [[nodiscard]]
        FramebufferRef GetFramebuffer() override;
    };
}

#endif // SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSRENDERPASS_HPP
