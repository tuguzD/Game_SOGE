#ifndef SOGE_GRAPHICS_DEFERRED_POINTLIGHTGRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_DEFERRED_POINTLIGHTGRAPHICSPIPELINE_HPP

#include "SOGE/Graphics/Deferred/GeometryGraphicsRenderPass.hpp"
#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GraphicsPipeline.hpp"


namespace soge
{
    class PointLightGraphicsPipelineEntity;

    class PointLightGraphicsPipeline : public GraphicsPipeline<PointLightGraphicsPipelineEntity>
    {
    private:
        struct ConstantBufferData
        {
            glm::mat4x4 m_viewProjection;
            glm::mat4x4 m_invProjection;
            glm::mat4x4 m_invView;
            glm::vec3 m_viewPosition;
        };

        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsRenderPass> m_geometryRenderPass;
        eastl::reference_wrapper<FinalGraphicsRenderPass> m_finalRenderPass;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;

        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;
        nvrhi::BindingLayoutHandle m_nvrhiEntityBindingLayout;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;
        nvrhi::ShaderHandle m_nvrhiPixelShader;
        nvrhi::ShaderHandle m_nvrhiVertexShader;

    public:
        explicit PointLightGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryRenderPass,
                                            FinalGraphicsRenderPass& aFinalRenderPass);

        [[nodiscard]]
        nvrhi::IBindingLayout& GetEntityBindingLayout();

        void WriteConstantBuffer(const Camera& aCamera, nvrhi::ICommandList& aCommandList);

        [[nodiscard]]
        nvrhi::IGraphicsPipeline& GetGraphicsPipeline() override;

        void Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                     nvrhi::ICommandList& aCommandList) override;
    };

    class PointLightGraphicsPipelineEntity
    {
    public:
        struct Tag
        {
        };

        struct ConstantBufferData
        {
            struct Attenuation
            {
                alignas(16) glm::vec3 m_constFactor;
                alignas(16) glm::vec3 m_linearFactor;
                alignas(16) glm::vec3 m_quadraticFactor;
            };

            glm::mat4x4 m_model;
            glm::vec3 m_color;
            float m_intensity;
            alignas(16) glm::vec3 m_position;
            Attenuation m_attenuation;
        };

        struct Vertex
        {
            glm::vec3 m_position;
        };

        using Index = std::uint32_t;

        constexpr explicit PointLightGraphicsPipelineEntity() noexcept = default;

        constexpr PointLightGraphicsPipelineEntity(const PointLightGraphicsPipelineEntity&) = delete;
        constexpr PointLightGraphicsPipelineEntity& operator=(const PointLightGraphicsPipelineEntity&) = delete;

        constexpr PointLightGraphicsPipelineEntity(PointLightGraphicsPipelineEntity&&) noexcept = default;
        constexpr PointLightGraphicsPipelineEntity& operator=(PointLightGraphicsPipelineEntity&&) noexcept = default;

        constexpr virtual ~PointLightGraphicsPipelineEntity() = default;

        [[nodiscard]]
        constexpr virtual nvrhi::BindingSetHandle GetBindingSet(Tag) = 0;
        [[nodiscard]]
        constexpr virtual nvrhi::BufferHandle GetVertexBuffer(Tag) = 0;
        [[nodiscard]]
        constexpr virtual nvrhi::BufferHandle GetIndexBuffer(Tag) = 0;

        constexpr virtual void WriteConstantBuffer(Tag, nvrhi::ICommandList& aCommandList) = 0;
        constexpr virtual void WriteVertexBuffer(Tag, nvrhi::ICommandList& aCommandList) = 0;
        constexpr virtual void WriteIndexBuffer(Tag, nvrhi::ICommandList& aCommandList) = 0;
    };
}

SOGE_DI_REGISTER_NS(
    soge, PointLightGraphicsPipeline,
    df::Single<PointLightGraphicsPipeline, GraphicsCore, GeometryGraphicsRenderPass, FinalGraphicsRenderPass>)

#endif // SOGE_GRAPHICS_DEFERRED_POINTLIGHTGRAPHICSPIPELINE_HPP
