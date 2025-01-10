#ifndef SOGE_GRAPHICS_TRIANGLEGRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_TRIANGLEGRAPHICSPIPELINE_HPP

#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/GraphicsPipeline.hpp"

#include <glm/vec4.hpp>


namespace soge
{
    class TriangleGraphicsPipeline : public GraphicsPipeline
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<FinalGraphicsRenderPass> m_renderPass;

        eastl::vector<nvrhi::CommandListHandle> m_commandLists;
        eastl::vector<CommandListRef> m_commandListRefs;

        nvrhi::ShaderHandle m_nvrhiVertexShader;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;
        nvrhi::ShaderHandle m_nvrhiPixelShader;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;

        // TODO: move this into component class which can pass data to multiple pipelines
        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BindingSetHandle m_nvrhiBindingSet;

    public:
        struct Vertex
        {
            glm::vec4 m_position;
            glm::vec4 m_color;
        };

        explicit TriangleGraphicsPipeline(GraphicsCore& aCore, FinalGraphicsRenderPass& aRenderPass);

        TriangleGraphicsPipeline(const TriangleGraphicsPipeline&) = delete;
        TriangleGraphicsPipeline& operator=(const TriangleGraphicsPipeline&) = delete;

        TriangleGraphicsPipeline(TriangleGraphicsPipeline&& aOther) noexcept;
        TriangleGraphicsPipeline& operator=(TriangleGraphicsPipeline&& aOther) noexcept;

        ~TriangleGraphicsPipeline() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(TriangleGraphicsPipeline& aOther) noexcept;

        [[nodiscard]]
        CommandLists Update(float aDeltaTime) override;
    };
}

SOGE_DI_REGISTER_NS(soge, TriangleGraphicsPipeline,
                    df::Single<TriangleGraphicsPipeline, GraphicsCore, FinalGraphicsRenderPass>)

#endif // SOGE_GRAPHICS_TRIANGLEGRAPHICSPIPELINE_HPP
