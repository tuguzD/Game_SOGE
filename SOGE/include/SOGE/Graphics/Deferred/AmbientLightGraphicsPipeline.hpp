#ifndef SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTGRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTGRAPHICSPIPELINE_HPP

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

        nvrhi::ShaderHandle m_nvrhiVertexShader;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;
        nvrhi::ShaderHandle m_nvrhiPixelShader;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::BindingLayoutHandle m_nvrhiEntityBindingLayout;
        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;

        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;
        nvrhi::BindingSetHandle m_nvrhiBindingSet;

    public:
        explicit AmbientLightGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryRenderPass,
                                              FinalGraphicsRenderPass& aFinalRenderPass);

        AmbientLightGraphicsPipeline(const AmbientLightGraphicsPipeline&) = delete;
        AmbientLightGraphicsPipeline& operator=(const AmbientLightGraphicsPipeline&) = delete;

        AmbientLightGraphicsPipeline(AmbientLightGraphicsPipeline&& aOther) noexcept;
        AmbientLightGraphicsPipeline& operator=(AmbientLightGraphicsPipeline&& aOther) noexcept;

        ~AmbientLightGraphicsPipeline() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(AmbientLightGraphicsPipeline& aOther) noexcept;

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

        struct ConstantBuffer
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
        [[nodiscard]]
        constexpr virtual nvrhi::BufferHandle GetConstantBuffer(Tag) = 0;
    };
}

SOGE_DI_REGISTER_NS(
    soge, AmbientLightGraphicsPipeline,
    df::Single<AmbientLightGraphicsPipeline, GraphicsCore, GeometryGraphicsRenderPass, FinalGraphicsRenderPass>)

#endif // SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTGRAPHICSPIPELINE_HPP
