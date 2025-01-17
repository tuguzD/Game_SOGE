#ifndef SOGE_GRAPHICS_TRIANGLEENTITY_HPP
#define SOGE_GRAPHICS_TRIANGLEENTITY_HPP

#include "SOGE/Graphics/TriangleGraphicsPipeline.hpp"


namespace soge
{
    class TriangleEntity : public TriangleGraphicsPipeline::Entity
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<TriangleGraphicsPipeline> m_pipeline;

        Transform m_transform;

        nvrhi::BufferHandle m_nvrhiConstantBuffer;
        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;
        nvrhi::BindingSetHandle m_nvrhiBindingSet;

    public:
        using Index = std::uint32_t;

        explicit TriangleEntity(GraphicsCore& aCore, TriangleGraphicsPipeline& aPipeline);

        [[nodiscard]]
        Transform& GetTransform();
        [[nodiscard]]
        const Transform& GetTransform() const;

        using Vertices = eastl::span<const Vertex>;
        void UpdateVertices(Vertices aVertices);

        using Indices = eastl::span<const Index>;
        void UpdateIndices(Indices aIndices);

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet() override;
        [[nodiscard]]
        nvrhi::BufferHandle GetConstantBuffer() override;
        [[nodiscard]]
        nvrhi::BufferHandle GetVertexBuffer() override;
        [[nodiscard]]
        nvrhi::BufferHandle GetIndexBuffer() override;

        [[nodiscard]]
        glm::mat4x4 GetWorldMatrix() override;
    };
}

SOGE_DI_REGISTER_NS(soge, TriangleEntity, df::Factory<TriangleEntity, GraphicsCore, TriangleGraphicsPipeline>)

#endif // SOGE_GRAPHICS_TRIANGLEENTITY_HPP
