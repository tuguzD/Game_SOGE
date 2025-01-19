#ifndef SOGE_GRAPHICS_ENTITIES_GEOMETRYENTITY_HPP
#define SOGE_GRAPHICS_ENTITIES_GEOMETRYENTITY_HPP

#include "SOGE/Graphics/Deferred/GeometryGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class GeometryEntity : public GraphicsEntity, public GeometryGraphicsPipelineEntity
    {
    private:
        void WriteConstantBuffer(nvrhi::ICommandList& aCommandList);
        void WriteVertexBuffer(nvrhi::ICommandList& aCommandList);
        void WriteIndexBuffer(nvrhi::ICommandList& aCommandList);

        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsPipeline> m_pipeline;

        Transform m_transform;
        eastl::vector<Vertex> m_vertices;
        eastl::vector<Index> m_indices;

        bool m_shouldWriteConstantBuffer;
        bool m_shouldWriteVertexBuffer;
        bool m_shouldWriteIndexBuffer;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;
        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;

    public:
        explicit GeometryEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                Transform aTransform = Transform{}, eastl::vector<Vertex> aVertices = {},
                                eastl::vector<Index> aIndices = {});

        [[nodiscard]]
        const Transform& GetTransform() const;
        Transform& GetTransform();

        [[nodiscard]]
        eastl::span<const Vertex> GetVertices() const;
        eastl::vector<Vertex>& GetVertices();

        [[nodiscard]]
        eastl::span<const Index> GetIndices() const;
        eastl::vector<Index>& GetIndices();

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetVertexBuffer(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetIndexBuffer(Tag) override;

        void WriteResources(Tag, nvrhi::ICommandList& aCommandList) override;
    };
}

SOGE_DI_REGISTER_NS(soge, GeometryEntity, df::Factory<GeometryEntity, GraphicsCore, GeometryGraphicsPipeline>)

#endif // SOGE_GRAPHICS_ENTITIES_GEOMETRYENTITY_HPP
