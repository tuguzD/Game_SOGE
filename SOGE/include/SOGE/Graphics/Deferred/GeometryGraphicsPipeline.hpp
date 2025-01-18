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
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsRenderPass> m_renderPass;

        nvrhi::ShaderHandle m_nvrhiVertexShader;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;
        nvrhi::ShaderHandle m_nvrhiPixelShader;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;

    public:
        explicit GeometryGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aRenderPass);

        GeometryGraphicsPipeline(const GeometryGraphicsPipeline&) = delete;
        GeometryGraphicsPipeline& operator=(const GeometryGraphicsPipeline&) = delete;

        GeometryGraphicsPipeline(GeometryGraphicsPipeline&& aOther) noexcept;
        GeometryGraphicsPipeline& operator=(GeometryGraphicsPipeline&& aOther) noexcept;

        ~GeometryGraphicsPipeline() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(GeometryGraphicsPipeline& aOther) noexcept;

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

        struct ConstantBuffer
        {
            glm::mat4x4 m_model;
            glm::mat4x4 m_view;
            glm::mat4x4 m_projection;
        };

        struct Vertex
        {
            alignas(16) glm::vec3 m_position;
            alignas(16) glm::vec3 m_normal;
            glm::vec4 m_color{1.0f};
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
        constexpr virtual nvrhi::BufferHandle GetConstantBuffer(Tag) = 0;
        [[nodiscard]]
        constexpr virtual nvrhi::BufferHandle GetVertexBuffer(Tag) = 0;
        [[nodiscard]]
        constexpr virtual nvrhi::BufferHandle GetIndexBuffer(Tag) = 0;

        [[nodiscard]]
        constexpr virtual glm::mat4x4 GetWorldMatrix(Tag) = 0;
    };
}

SOGE_DI_REGISTER_NS(soge, GeometryGraphicsPipeline,
                    df::Single<GeometryGraphicsPipeline, GraphicsCore, GeometryGraphicsRenderPass>)

#endif // SOGE_GRAPHICS_DEFERRED_GEOMETRYGRAPHICSPIPELINE_HPP
