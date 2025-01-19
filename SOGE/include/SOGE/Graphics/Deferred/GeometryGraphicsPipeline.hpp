#ifndef SOGE_GRAPHICS_DEFERRED_GEOMETRYGRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_DEFERRED_GEOMETRYGRAPHICSPIPELINE_HPP

#include "SOGE/Graphics/Deferred/GeometryGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/GraphicsPipeline.hpp"


namespace soge
{
    class GeometryGraphicsPipelineEntity;

    class GeometryGraphicsPipeline : public GraphicsPipeline<GeometryGraphicsPipelineEntity>
    {
    private:
        struct ConstantBufferData
        {
            glm::mat4x4 m_viewProjection;
        };

        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsRenderPass> m_renderPass;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;

        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;
        nvrhi::BindingLayoutHandle m_nvrhiEntityBindingLayout;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;
        nvrhi::ShaderHandle m_nvrhiPixelShader;
        nvrhi::ShaderHandle m_nvrhiVertexShader;

    public:
        explicit GeometryGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aRenderPass);

        [[nodiscard]]
        nvrhi::IBindingLayout& GetEntityBindingLayout();

        void WriteConstantBuffer(const Camera& aCamera, nvrhi::ICommandList& aCommandList);

        [[nodiscard]]
        nvrhi::IGraphicsPipeline& GetGraphicsPipeline() noexcept override;

        void Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                     nvrhi::ICommandList& aCommandList) override;
    };

    class GeometryGraphicsPipelineEntity
    {
    public:
        struct Tag
        {
        };

        struct ConstantBufferData
        {
            glm::mat4x4 m_model;
        };

        struct Vertex
        {
            alignas(16) glm::vec3 m_position;
            alignas(16) glm::vec3 m_normal;
            alignas(16) glm::vec3 m_color{1.0f};
        };

        using Index = std::uint32_t;

        constexpr explicit GeometryGraphicsPipelineEntity() noexcept = default;

        constexpr GeometryGraphicsPipelineEntity(const GeometryGraphicsPipelineEntity&) = delete;
        constexpr GeometryGraphicsPipelineEntity& operator=(const GeometryGraphicsPipelineEntity&) = delete;

        constexpr GeometryGraphicsPipelineEntity(GeometryGraphicsPipelineEntity&&) noexcept = default;
        constexpr GeometryGraphicsPipelineEntity& operator=(GeometryGraphicsPipelineEntity&&) noexcept = default;

        constexpr virtual ~GeometryGraphicsPipelineEntity() = default;

        [[nodiscard]]
        constexpr virtual nvrhi::BindingSetHandle GetBindingSet(Tag) = 0;
        [[nodiscard]]
        constexpr virtual nvrhi::BufferHandle GetVertexBuffer(Tag) = 0;
        [[nodiscard]]
        constexpr virtual nvrhi::BufferHandle GetIndexBuffer(Tag) = 0;

        constexpr virtual void WriteResources(Tag, nvrhi::ICommandList& aCommandList) = 0;
    };
}

SOGE_DI_REGISTER_NS(soge, GeometryGraphicsPipeline,
                    df::Single<GeometryGraphicsPipeline, GraphicsCore, GeometryGraphicsRenderPass>)

#endif // SOGE_GRAPHICS_DEFERRED_GEOMETRYGRAPHICSPIPELINE_HPP
