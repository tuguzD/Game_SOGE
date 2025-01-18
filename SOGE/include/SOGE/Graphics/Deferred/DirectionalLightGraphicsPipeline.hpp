#ifndef SOGE_GRAPHICS_DEFERRED_DIRECTIONALLIGHTGRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_DEFERRED_DIRECTIONALLIGHTGRAPHICSPIPELINE_HPP

#include "SOGE/Graphics/Deferred/GeometryGraphicsRenderPass.hpp"
#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GraphicsPipeline.hpp"


namespace soge
{
    class DirectionalLightGraphicsPipelineEntity;

    class DirectionalLightGraphicsPipeline : public GraphicsPipeline<DirectionalLightGraphicsPipelineEntity>
    {
    private:
        struct ConstantBufferData
        {
            glm::mat4x4 m_invProjection;
            glm::mat4x4 m_invView;
            glm::vec3 m_viewPosition;
        };

        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsRenderPass> m_geometryRenderPass;
        eastl::reference_wrapper<FinalGraphicsRenderPass> m_finalRenderPass;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;
        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;

        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;
        nvrhi::BindingLayoutHandle m_nvrhiEntityBindingLayout;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;
        nvrhi::ShaderHandle m_nvrhiPixelShader;
        nvrhi::ShaderHandle m_nvrhiVertexShader;

    public:
        explicit DirectionalLightGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryRenderPass,
                                                  FinalGraphicsRenderPass& aFinalRenderPass);

        [[nodiscard]]
        nvrhi::IBindingLayout& GetEntityBindingLayout();

        void WriteConstantBuffer(const Camera& aCamera, nvrhi::ICommandList& aCommandList);

        [[nodiscard]]
        nvrhi::IGraphicsPipeline& GetGraphicsPipeline() override;

        void Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                     nvrhi::ICommandList& aCommandList) override;
    };

    class DirectionalLightGraphicsPipelineEntity
    {
    public:
        struct Tag
        {
        };

        struct ConstantBufferData
        {
            glm::vec3 m_color;
            float m_intensity;
            glm::vec3 m_direction;
        };

        constexpr explicit DirectionalLightGraphicsPipelineEntity() noexcept = default;

        constexpr DirectionalLightGraphicsPipelineEntity(const DirectionalLightGraphicsPipelineEntity&) = delete;
        constexpr DirectionalLightGraphicsPipelineEntity& operator=(const DirectionalLightGraphicsPipelineEntity&) =
            delete;

        constexpr DirectionalLightGraphicsPipelineEntity(DirectionalLightGraphicsPipelineEntity&&) noexcept = default;
        constexpr DirectionalLightGraphicsPipelineEntity& operator=(DirectionalLightGraphicsPipelineEntity&&) noexcept =
            default;

        constexpr virtual ~DirectionalLightGraphicsPipelineEntity() = default;

        [[nodiscard]]
        constexpr virtual nvrhi::BindingSetHandle GetBindingSet(Tag) = 0;

        constexpr virtual void WriteConstantBuffer(Tag, nvrhi::ICommandList& aCommandList) = 0;
    };
}

SOGE_DI_REGISTER_NS(
    soge, DirectionalLightGraphicsPipeline,
    df::Single<DirectionalLightGraphicsPipeline, GraphicsCore, GeometryGraphicsRenderPass, FinalGraphicsRenderPass>)

#endif // SOGE_GRAPHICS_DEFERRED_DIRECTIONALLIGHTGRAPHICSPIPELINE_HPP
