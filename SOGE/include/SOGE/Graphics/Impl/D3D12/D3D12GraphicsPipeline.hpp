#ifndef SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSPIPELINE_HPP

#include "SOGE/Graphics/GraphicsPipeline.hpp"

#include <EASTL/vector.h>
#include <glm/vec4.hpp>
#include <nvrhi/nvrhi.h>


namespace soge
{
    class D3D12GraphicsCore;

    class D3D12GraphicsPipeline : public GraphicsPipeline
    {
    private:
        eastl::reference_wrapper<D3D12GraphicsCore> m_core;

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

        explicit D3D12GraphicsPipeline(D3D12GraphicsCore& aCore);

        D3D12GraphicsPipeline(const D3D12GraphicsPipeline&) = delete;
        D3D12GraphicsPipeline& operator=(const D3D12GraphicsPipeline&) = delete;

        D3D12GraphicsPipeline(D3D12GraphicsPipeline&& aOther) noexcept;
        D3D12GraphicsPipeline& operator=(D3D12GraphicsPipeline&& aOther) noexcept;

        ~D3D12GraphicsPipeline() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(D3D12GraphicsPipeline& aOther) noexcept;

        [[nodiscard]]
        CommandLists Update(float aDeltaTime) override;
    };
}

#endif // SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSPIPELINE_HPP
