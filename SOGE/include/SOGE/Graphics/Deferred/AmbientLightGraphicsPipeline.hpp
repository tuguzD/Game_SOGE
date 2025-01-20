#ifndef SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTGRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTGRAPHICSPIPELINE_HPP

#include "SOGE/Graphics/Deferred/AmbientLightPixelShaderResource.hpp"
#include "SOGE/Graphics/Deferred/AmbientLightVertexShaderResource.hpp"
#include "SOGE/Graphics/Deferred/GeometryGraphicsRenderPass.hpp"
#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GraphicsPipeline.hpp"


namespace soge
{
    class AmbientLightGraphicsPipelineEntity;

    class AmbientLightGraphicsPipeline : public GraphicsPipeline<AmbientLightGraphicsPipelineEntity>
    {
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsRenderPass> m_geometryRenderPass;
        eastl::reference_wrapper<FinalGraphicsRenderPass> m_finalRenderPass;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;
        nvrhi::BufferHandle m_nvrhiVertexBuffer;

        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;
        nvrhi::BindingLayoutHandle m_nvrhiEntityBindingLayout;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;

    public:
        explicit AmbientLightGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryRenderPass,
                                              FinalGraphicsRenderPass& aFinalRenderPass,
                                              AmbientLightVertexShaderResource& aVertexShader,
                                              AmbientLightPixelShaderResource& aPixelShader);

        [[nodiscard]]
        nvrhi::IBindingLayout& GetEntityBindingLayout();

        [[nodiscard]]
        nvrhi::IGraphicsPipeline& GetGraphicsPipeline() override;

        void Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                     nvrhi::ICommandList& aCommandList) override;
    };

    class AmbientLightGraphicsPipelineEntity
    {
    public:
        struct Tag
        {
        };

        struct ConstantBufferData
        {
            glm::vec3 m_color;
            float m_intensity;
        };

        constexpr explicit AmbientLightGraphicsPipelineEntity() noexcept = default;

        constexpr AmbientLightGraphicsPipelineEntity(const AmbientLightGraphicsPipelineEntity&) = delete;
        constexpr AmbientLightGraphicsPipelineEntity& operator=(const AmbientLightGraphicsPipelineEntity&) = delete;

        constexpr AmbientLightGraphicsPipelineEntity(AmbientLightGraphicsPipelineEntity&&) noexcept = default;
        constexpr AmbientLightGraphicsPipelineEntity& operator=(AmbientLightGraphicsPipelineEntity&&) noexcept =
            default;

        constexpr virtual ~AmbientLightGraphicsPipelineEntity() = default;

        [[nodiscard]]
        constexpr virtual nvrhi::BindingSetHandle GetBindingSet(Tag) = 0;

        constexpr virtual void WriteResources(Tag, nvrhi::ICommandList& aCommandList) = 0;
    };
}

SOGE_DI_REGISTER_NS(
    soge, AmbientLightGraphicsPipeline,
    df::Single<AmbientLightGraphicsPipeline, GraphicsCore, GeometryGraphicsRenderPass, FinalGraphicsRenderPass,
               AmbientLightVertexShaderResource, AmbientLightPixelShaderResource>)

#endif // SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTGRAPHICSPIPELINE_HPP
