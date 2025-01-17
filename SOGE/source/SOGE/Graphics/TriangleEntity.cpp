#include "sogepch.hpp"

#include "SOGE/Graphics/TriangleEntity.hpp"

#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"


namespace soge
{
    TriangleEntity::TriangleEntity(GraphicsCore& aCore, TriangleGraphicsPipeline& aTrianglePipeline,
                                   GeometryGraphicsPipeline& aGeometryPipeline)
        : m_core{aCore}, m_trianglePipeline{aTrianglePipeline}, m_geometryPipeline{aGeometryPipeline}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        SOGE_INFO_LOG("Creating NVRHI constant buffer for triangle entity...");
        nvrhi::BufferDesc bufferDesc{};
        bufferDesc.byteSize = sizeof(ConstantBuffer);
        bufferDesc.isConstantBuffer = true;
        bufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        bufferDesc.keepInitialState = true;
        bufferDesc.debugName = "SOGE triangle entity constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(bufferDesc);

        {
            SOGE_INFO_LOG("Creating NVRHI binding set for triangle entity (triangle pipeline)...");
            nvrhi::BindingSetDesc triangleBindingSetDesc{};
            triangleBindingSetDesc.trackLiveness = true;
            triangleBindingSetDesc.addItem(nvrhi::BindingSetItem::ConstantBuffer(0, m_nvrhiConstantBuffer));

            const auto triangleBindingLayout = aTrianglePipeline.GetGraphicsPipeline().getDesc().bindingLayouts[0];
            m_nvrhiTriangleBindingSet = device.createBindingSet(triangleBindingSetDesc, triangleBindingLayout);
        }

        {
            SOGE_INFO_LOG("Creating NVRHI binding set for triangle entity (geometry pipeline)...");
            nvrhi::BindingSetDesc geometryBindingSetDesc{};
            geometryBindingSetDesc.trackLiveness = true;
            geometryBindingSetDesc.addItem(nvrhi::BindingSetItem::ConstantBuffer(0, m_nvrhiConstantBuffer));

            const auto geometryBindingLayout = aGeometryPipeline.GetGraphicsPipeline().getDesc().bindingLayouts[0];
            m_nvrhiGeometryBindingSet = device.createBindingSet(geometryBindingSetDesc, geometryBindingLayout);
        }
    }

    Transform& TriangleEntity::GetTransform()
    {
        return m_transform;
    }

    const Transform& TriangleEntity::GetTransform() const
    {
        return m_transform;
    }

    void TriangleEntity::UpdateVertices(const Vertices aVertices)
    {
        if (aVertices.empty())
        {
            m_nvrhiVertexBuffer = nullptr;
            return;
        }

        GraphicsCore& core = m_core.get();
        nvrhi::IDevice& device = core.GetRawDevice();
        const auto inputByteSize = static_cast<std::uint64_t>(aVertices.size_bytes());

        if (m_nvrhiVertexBuffer == nullptr || m_nvrhiVertexBuffer->getDesc().byteSize != inputByteSize)
        {
            SOGE_INFO_LOG("Creating NVRHI vertex buffer for triangle entity...");
            nvrhi::BufferDesc bufferDesc{};
            bufferDesc.byteSize = inputByteSize;
            bufferDesc.isVertexBuffer = true;
            bufferDesc.initialState = nvrhi::ResourceStates::VertexBuffer;
            bufferDesc.keepInitialState = true;
            bufferDesc.debugName = "SOGE triangle entity vertex buffer";
            m_nvrhiVertexBuffer = device.createBuffer(bufferDesc);
        }

        SOGE_INFO_LOG("Updating NVRHI vertex buffer for triangle entity...");
        const nvrhi::CommandListHandle updateCommandList = device.createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};
            commandList->writeBuffer(m_nvrhiVertexBuffer, aVertices.data(), inputByteSize);
        }
        core.ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    void TriangleEntity::UpdateIndices(const Indices aIndices)
    {
        if (aIndices.empty())
        {
            m_nvrhiIndexBuffer = nullptr;
            return;
        }

        GraphicsCore& core = m_core.get();
        nvrhi::IDevice& device = core.GetRawDevice();
        const auto inputByteSize = static_cast<std::uint64_t>(aIndices.size_bytes());

        if (m_nvrhiIndexBuffer == nullptr || m_nvrhiIndexBuffer->getDesc().byteSize != inputByteSize)
        {
            SOGE_INFO_LOG("Creating NVRHI index buffer for triangle entity...");
            nvrhi::BufferDesc bufferDesc{};
            bufferDesc.byteSize = inputByteSize;
            bufferDesc.isIndexBuffer = true;
            bufferDesc.initialState = nvrhi::ResourceStates::IndexBuffer;
            bufferDesc.keepInitialState = true;
            bufferDesc.debugName = "SOGE triangle entity index buffer";
            m_nvrhiIndexBuffer = device.createBuffer(bufferDesc);
        }

        SOGE_INFO_LOG("Updating NVRHI index buffer for triangle entity...");
        const nvrhi::CommandListHandle updateCommandList = device.createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};
            commandList->writeBuffer(m_nvrhiIndexBuffer, aIndices.data(), inputByteSize);
        }
        core.ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    nvrhi::BindingSetHandle TriangleEntity::GetBindingSet(TriangleTag)
    {
        return m_nvrhiTriangleBindingSet;
    }

    nvrhi::BufferHandle TriangleEntity::GetConstantBuffer(TriangleTag)
    {
        return m_nvrhiConstantBuffer;
    }

    nvrhi::BufferHandle TriangleEntity::GetVertexBuffer(TriangleTag)
    {
        return m_nvrhiVertexBuffer;
    }

    nvrhi::BufferHandle TriangleEntity::GetIndexBuffer(TriangleTag)
    {
        return m_nvrhiIndexBuffer;
    }

    glm::mat4x4 TriangleEntity::GetWorldMatrix(TriangleTag)
    {
        return m_transform.WorldMatrix();
    }

    nvrhi::BindingSetHandle TriangleEntity::GetBindingSet(GeometryTag)
    {
        return m_nvrhiGeometryBindingSet;
    }

    nvrhi::BufferHandle TriangleEntity::GetConstantBuffer(GeometryTag)
    {
        return m_nvrhiConstantBuffer;
    }

    nvrhi::BufferHandle TriangleEntity::GetVertexBuffer(GeometryTag)
    {
        return m_nvrhiVertexBuffer;
    }

    nvrhi::BufferHandle TriangleEntity::GetIndexBuffer(GeometryTag)
    {
        return m_nvrhiIndexBuffer;
    }

    glm::mat4x4 TriangleEntity::GetWorldMatrix(GeometryTag)
    {
        return m_transform.WorldMatrix();
    }
}
