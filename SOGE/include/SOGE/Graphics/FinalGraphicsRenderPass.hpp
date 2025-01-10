#ifndef SOGE_GRAPHICS_FINALGRAPHICSRENDERPASS_HPP
#define SOGE_GRAPHICS_FINALGRAPHICSRENDERPASS_HPP

#include "SOGE/Graphics/GraphicsRenderPass.hpp"

#include <EASTL/vector.h>


namespace soge
{
    class GraphicsCore;

    class FinalGraphicsRenderPass : public GraphicsRenderPass
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;

        eastl::vector<nvrhi::FramebufferHandle> m_nvrhiFramebuffers;

    public:
        explicit FinalGraphicsRenderPass(GraphicsCore& aCore);

        FinalGraphicsRenderPass(const FinalGraphicsRenderPass&) = delete;
        FinalGraphicsRenderPass& operator=(const FinalGraphicsRenderPass&) = delete;

        FinalGraphicsRenderPass(FinalGraphicsRenderPass&& aOther) noexcept;
        FinalGraphicsRenderPass& operator=(FinalGraphicsRenderPass&& aOther) noexcept;

        ~FinalGraphicsRenderPass() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(FinalGraphicsRenderPass& aOther) noexcept;

        [[nodiscard]]
        nvrhi::IFramebuffer& GetFramebuffer() override;
    };
}

#endif // SOGE_GRAPHICS_FINALGRAPHICSRENDERPASS_HPP
