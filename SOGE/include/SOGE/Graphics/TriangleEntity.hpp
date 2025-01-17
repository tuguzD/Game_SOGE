#ifndef SOGE_GRAPHICS_TRIANGLEENTITY_HPP
#define SOGE_GRAPHICS_TRIANGLEENTITY_HPP

#include "SOGE/Graphics/GeometryGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class TriangleEntity : public GraphicsEntity, public GeometryGraphicsPipeline::Entity
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsPipeline> m_geometryPipeline;

        Transform m_transform;

        nvrhi::BufferHandle m_nvrhiConstantBuffer;
        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;

    public:
        explicit TriangleEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aGeometryPipeline);

        [[nodiscard]]
        Transform& GetTransform();
        [[nodiscard]]
        const Transform& GetTransform() const;

        using Vertices = eastl::span<const Vertex>;
        void UpdateVertices(Vertices aVertices);

        using Indices = eastl::span<const Index>;
        void UpdateIndices(Indices aIndices);

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetConstantBuffer(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetVertexBuffer(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetIndexBuffer(Tag) override;
        [[nodiscard]]
        glm::mat4x4 GetWorldMatrix(Tag) override;
    };
}

SOGE_DI_REGISTER_NS(soge, TriangleEntity, df::Factory<TriangleEntity, GraphicsCore, GeometryGraphicsPipeline>)

#endif // SOGE_GRAPHICS_TRIANGLEENTITY_HPP
