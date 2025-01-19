#include "sogepch.hpp"
#include "SOGE/Graphics/Mesh.hpp"


namespace soge
{
    Mesh::Mesh(GraphicsCore& aGraphicsCore, GeometryGraphicsPipeline& aGeometryPipeline)
        : m_core(aGraphicsCore), m_geometryPipeline(aGeometryPipeline)
    {
    }

    void Mesh::CreateVertexBuffer(const eastl::vector<Vertex>& aVertices)
    {
        nvrhi::IDevice& device = m_core.get().GetRawDevice();

        nvrhi::BufferDesc vertexBufferDesc;
        vertexBufferDesc.byteSize           = m_vertices.size() * sizeof(Vertex);
        vertexBufferDesc.structStride       = sizeof(Vertex);
        vertexBufferDesc.initialState       = nvrhi::ResourceStates::VertexBuffer;
        vertexBufferDesc.keepInitialState   = true;
        vertexBufferDesc.debugName          = "VertexBuffer";
        m_vertexBuffer                      = device.createBuffer(vertexBufferDesc);
    }

    void Mesh::CreateIndexBuffer(const eastl::vector<Index>& aIndices)
    {
        nvrhi::IDevice& device = m_core.get().GetRawDevice();

        nvrhi::BufferDesc indexBufferDesc;
        indexBufferDesc.byteSize            = m_indices.size() + sizeof(Index);
        indexBufferDesc.structStride        = sizeof(Index);
        indexBufferDesc.initialState        = nvrhi::ResourceStates::IndexBuffer;
        indexBufferDesc.keepInitialState    = true;
        indexBufferDesc.debugName           = "IndexBuffer";
        m_indexBuffer                       = device.createBuffer(indexBufferDesc);
    }
}
