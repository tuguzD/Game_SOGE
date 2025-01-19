#ifndef SOGE_GRAPHICS_ENTITIES_TRIANGLEENTITY_HPP
#define SOGE_GRAPHICS_ENTITIES_TRIANGLEENTITY_HPP

#include "SOGE/Graphics/Deferred/GeometryGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class GeometryEntity : public GraphicsEntity, public GeometryGraphicsPipeline::Entity
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsPipeline> m_pipeline;

        Transform m_transform;
        bool m_shouldWrite;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;
        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;

    public:
        explicit GeometryEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                Transform aTransform = Transform{});

        [[nodiscard]]
        const Transform& GetTransform() const;
        Transform& GetTransform();

        using Vertices = eastl::span<const Vertex>;
        void UpdateVertices(Vertices aVertices);

        using Indices = eastl::span<const Index>;
        void UpdateIndices(Indices aIndices);

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetVertexBuffer(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetIndexBuffer(Tag) override;

        void WriteConstantBuffer(Tag, nvrhi::ICommandList& aCommandList) override;
        void WriteVertexBuffer(Tag, nvrhi::ICommandList& aCommandList) override;
        void WriteIndexBuffer(Tag, nvrhi::ICommandList& aCommandList) override;
    };
}

SOGE_DI_REGISTER_NS(soge, GeometryEntity, df::Factory<GeometryEntity, GraphicsCore, GeometryGraphicsPipeline>)

#endif // SOGE_GRAPHICS_ENTITIES_TRIANGLEENTITY_HPP
