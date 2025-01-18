#ifndef SOGE_GRAPHICS_FINALGRAPHICSRENDERPASS_HPP
#define SOGE_GRAPHICS_FINALGRAPHICSRENDERPASS_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/GraphicsRenderPass.hpp"

#include <EASTL/vector.h>


namespace soge
{
    class FinalGraphicsRenderPass : public GraphicsRenderPass
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;

        eastl::vector<nvrhi::FramebufferHandle> m_nvrhiFramebuffers;

    public:
        explicit FinalGraphicsRenderPass(GraphicsCore& aCore);

        void ClearFramebuffer(nvrhi::ICommandList& aCommandList);

        [[nodiscard]]
        nvrhi::IFramebuffer& GetFramebuffer() override;
    };
}

SOGE_DI_REGISTER_NS(soge, FinalGraphicsRenderPass, df::Single<FinalGraphicsRenderPass, GraphicsCore>)

#endif // SOGE_GRAPHICS_FINALGRAPHICSRENDERPASS_HPP
