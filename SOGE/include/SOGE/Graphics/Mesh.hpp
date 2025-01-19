#ifndef SOGE_GRAPHICS_MESH_HPP
#define SOGE_GRAPHICS_MESH_HPP

#include "SOGE/Graphics/Deferred/GeometryGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/Vertex.hpp"

#include <nvrhi/nvrhi.h>
#include <NRI.h>


namespace soge
{
    class Mesh
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsPipeline> m_geometryPipeline;

        eastl::vector<Vertex> m_vertices;
        eastl::vector<std::uint32_t> m_indices;

        nvrhi::BufferHandle m_vertexBuffer;
        nvrhi::BufferHandle m_indexBuffer;

    public:
        explicit Mesh(GraphicsCore& aGraphicsCore, GeometryGraphicsPipeline& aGeometryPipeline);

        void CreateVertexBuffer(const eastl::vector<Vertex>& aVertices);
        void CreateIndexBuffer(const eastl::vector<Index>& aIndices);
        void CreateConstantBuffer();

    };
}

#endif // !SOGE_GRAPHICS_MESH_HPP
