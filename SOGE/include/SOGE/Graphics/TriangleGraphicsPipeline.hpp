#ifndef SOGE_GRAPHICS_TRIANGLEGRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_TRIANGLEGRAPHICSPIPELINE_HPP

#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/GraphicsPipeline.hpp"


namespace soge
{
    class TriangleGraphicsPipelineEntity;

    class TriangleGraphicsPipeline : public GraphicsPipeline<TriangleGraphicsPipelineEntity>
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<FinalGraphicsRenderPass> m_renderPass;

        nvrhi::ShaderHandle m_nvrhiVertexShader;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;
        nvrhi::ShaderHandle m_nvrhiPixelShader;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;

    public:
        explicit TriangleGraphicsPipeline(GraphicsCore& aCore, FinalGraphicsRenderPass& aRenderPass);

        TriangleGraphicsPipeline(const TriangleGraphicsPipeline&) = delete;
        TriangleGraphicsPipeline& operator=(const TriangleGraphicsPipeline&) = delete;

        TriangleGraphicsPipeline(TriangleGraphicsPipeline&& aOther) noexcept;
        TriangleGraphicsPipeline& operator=(TriangleGraphicsPipeline&& aOther) noexcept;

        ~TriangleGraphicsPipeline() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(TriangleGraphicsPipeline& aOther) noexcept;

        [[nodiscard]]
        nvrhi::IGraphicsPipeline& GetGraphicsPipeline() noexcept override;

        void Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                     nvrhi::ICommandList& aCommandList) override;
    };

    class TriangleGraphicsPipelineEntity
    {
    public:
        struct Tag
        {
        };

        struct ConstantBuffer
        {
            glm::mat4x4 m_modelViewProjection;
        };

        struct Vertex
        {
            alignas(16) glm::vec3 m_position;
            glm::vec4 m_color;
        };

        using Index = std::uint32_t;

        constexpr explicit TriangleGraphicsPipelineEntity() noexcept = default;

        constexpr TriangleGraphicsPipelineEntity(const TriangleGraphicsPipelineEntity&) = delete;
        constexpr TriangleGraphicsPipelineEntity& operator=(const TriangleGraphicsPipelineEntity&) = delete;

        constexpr TriangleGraphicsPipelineEntity(TriangleGraphicsPipelineEntity&&) noexcept = default;
        constexpr TriangleGraphicsPipelineEntity& operator=(TriangleGraphicsPipelineEntity&&) noexcept = default;

        constexpr virtual ~TriangleGraphicsPipelineEntity() = default;

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

SOGE_DI_REGISTER_NS(soge, TriangleGraphicsPipeline,
                    df::Single<TriangleGraphicsPipeline, GraphicsCore, FinalGraphicsRenderPass>)

#endif // SOGE_GRAPHICS_TRIANGLEGRAPHICSPIPELINE_HPP
