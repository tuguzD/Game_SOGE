#ifndef SOGE_GRAPHICS_DEFERRED_LIGHTGRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_DEFERRED_LIGHTGRAPHICSPIPELINE_HPP

#include "SOGE/Graphics/Deferred/GeometryGraphicsRenderPass.hpp"
#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GraphicsPipeline.hpp"


namespace soge
{
    class LightGraphicsPipelineEntity;

    class LightGraphicsPipeline : public GraphicsPipeline<LightGraphicsPipelineEntity>
    {
    private:
        struct ConstantBuffer
        {
            glm::mat4x4 m_invProjection;
            glm::mat4x4 m_invView;
        };

        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsRenderPass> m_geometryRenderPass;
        eastl::reference_wrapper<FinalGraphicsRenderPass> m_finalRenderPass;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;
        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;

        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;
        nvrhi::ShaderHandle m_nvrhiPixelShader;
        nvrhi::ShaderHandle m_nvrhiVertexShader;

    public:
        explicit LightGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryRenderPass,
                                       FinalGraphicsRenderPass& aFinalRenderPass);

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

#endif // SOGE_GRAPHICS_DEFERRED_LIGHTGRAPHICSPIPELINE_HPP
