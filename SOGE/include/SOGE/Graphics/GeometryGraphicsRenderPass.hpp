#ifndef SOGE_GRAPHICS_GEOMETRYGRAPHICSRENDERPASS_HPP
#define SOGE_GRAPHICS_GEOMETRYGRAPHICSRENDERPASS_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/GraphicsRenderPass.hpp"


namespace soge
{
    class GeometryGraphicsRenderPass : public GraphicsRenderPass
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;

        nvrhi::TextureHandle m_nvrhiAlbedoTexture;

        nvrhi::FramebufferHandle m_nvrhiFramebuffer;

    public:
        explicit GeometryGraphicsRenderPass(GraphicsCore& aCore);

        GeometryGraphicsRenderPass(const GeometryGraphicsRenderPass&) = delete;
        GeometryGraphicsRenderPass& operator=(const GeometryGraphicsRenderPass&) = delete;

        GeometryGraphicsRenderPass(GeometryGraphicsRenderPass&& aOther) noexcept;
        GeometryGraphicsRenderPass& operator=(GeometryGraphicsRenderPass&& aOther) noexcept;

        ~GeometryGraphicsRenderPass() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(GeometryGraphicsRenderPass& aOther) noexcept;

        void ClearFramebuffer(nvrhi::ICommandList& aCommandList);

        [[nodiscard]]
        nvrhi::ITexture& GetAlbedoTexture();

        [[nodiscard]]
        nvrhi::IFramebuffer& GetFramebuffer() override;
    };
}

SOGE_DI_REGISTER_NS(soge, GeometryGraphicsRenderPass, df::Single<GeometryGraphicsRenderPass, GraphicsCore>)

#endif // SOGE_GRAPHICS_GEOMETRYGRAPHICSRENDERPASS_HPP
