#ifndef SOGE_GRAPHICS_LIGHTGRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_LIGHTGRAPHICSPIPELINE_HPP

#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GeometryGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GraphicsPipeline.hpp"


namespace soge
{
    class LightGraphicsPipelineEntity;

    class LightGraphicsPipeline : public GraphicsPipeline<LightGraphicsPipelineEntity>
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsRenderPass> m_geometryRenderPass;
        eastl::reference_wrapper<FinalGraphicsRenderPass> m_finalRenderPass;

        nvrhi::ShaderHandle m_nvrhiVertexShader;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;
        nvrhi::ShaderHandle m_nvrhiPixelShader;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;

        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;
        nvrhi::BindingSetHandle m_nvrhiBindingSet;

    public:
        explicit LightGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryRenderPass,
                                       FinalGraphicsRenderPass& aFinalRenderPass);

        LightGraphicsPipeline(const LightGraphicsPipeline&) = delete;
        LightGraphicsPipeline& operator=(const LightGraphicsPipeline&) = delete;

        LightGraphicsPipeline(LightGraphicsPipeline&& aOther) noexcept;
        LightGraphicsPipeline& operator=(LightGraphicsPipeline&& aOther) noexcept;

        ~LightGraphicsPipeline() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(LightGraphicsPipeline& aOther) noexcept;

        [[nodiscard]]
        nvrhi::IGraphicsPipeline& GetGraphicsPipeline() override;

        void Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                     nvrhi::ICommandList& aCommandList) override;
    };

    class LightGraphicsPipelineEntity
    {
    };
}

SOGE_DI_REGISTER_NS(
    soge, LightGraphicsPipeline,
    df::Single<LightGraphicsPipeline, GraphicsCore, GeometryGraphicsRenderPass, FinalGraphicsRenderPass>)

#endif // SOGE_GRAPHICS_LIGHTGRAPHICSPIPELINE_HPP
