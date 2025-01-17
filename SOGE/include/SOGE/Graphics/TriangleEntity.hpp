#ifndef SOGE_GRAPHICS_TRIANGLEENTITY_HPP
#define SOGE_GRAPHICS_TRIANGLEENTITY_HPP

#include "SOGE/Graphics/GeometryGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"
#include "SOGE/Graphics/TriangleGraphicsPipeline.hpp"


namespace soge
{
    class TriangleEntity : public GraphicsEntity,
                           public TriangleGraphicsPipeline::Entity,
                           public GeometryGraphicsPipeline::Entity
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<TriangleGraphicsPipeline> m_trianglePipeline;
        eastl::reference_wrapper<GeometryGraphicsPipeline> m_geometryPipeline;

        Transform m_transform;

        nvrhi::BufferHandle m_nvrhiConstantBuffer;
        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;

        nvrhi::BindingSetHandle m_nvrhiTriangleBindingSet;
        nvrhi::BindingSetHandle m_nvrhiGeometryBindingSet;

    public:
        using ConstantBuffer = TriangleGraphicsPipeline::Entity::ConstantBuffer;
        using Vertex = TriangleGraphicsPipeline::Entity::Vertex;
        using Index = TriangleGraphicsPipeline::Entity::Index;

        explicit TriangleEntity(GraphicsCore& aCore, TriangleGraphicsPipeline& aTrianglePipeline,
                                GeometryGraphicsPipeline& aGeometryPipeline);

        [[nodiscard]]
        Transform& GetTransform();
        [[nodiscard]]
        const Transform& GetTransform() const;

        using Vertices = eastl::span<const Vertex>;
        void UpdateVertices(Vertices aVertices);

        using Indices = eastl::span<const Index>;
        void UpdateIndices(Indices aIndices);

        using TriangleTag = TriangleGraphicsPipeline::Entity::Tag;

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet(TriangleTag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetConstantBuffer(TriangleTag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetVertexBuffer(TriangleTag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetIndexBuffer(TriangleTag) override;
        [[nodiscard]]
        glm::mat4x4 GetWorldMatrix(TriangleTag) override;

        using GeometryTag = GeometryGraphicsPipeline::Entity::Tag;

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet(GeometryTag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetConstantBuffer(GeometryTag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetVertexBuffer(GeometryTag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetIndexBuffer(GeometryTag) override;
        [[nodiscard]]
        glm::mat4x4 GetWorldMatrix(GeometryTag) override;
    };
}

SOGE_DI_REGISTER_NS(soge, TriangleEntity,
                    df::Factory<TriangleEntity, GraphicsCore, TriangleGraphicsPipeline, GeometryGraphicsPipeline>)

#endif // SOGE_GRAPHICS_TRIANGLEENTITY_HPP
