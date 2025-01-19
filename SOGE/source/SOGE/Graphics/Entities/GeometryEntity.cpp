#include "sogepch.hpp"

#include "SOGE/Graphics/Entities/GeometryEntity.hpp"

#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"


namespace soge
{
    GeometryEntity::GeometryEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline, Transform aTransform)
        : m_core{aCore}, m_pipeline{aPipeline}, m_transform{aTransform}, m_shouldWrite{true}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        SOGE_INFO_LOG("Creating NVRHI constant buffer for geometry entity...");
        nvrhi::BufferDesc constantBufferDesc{};
        constantBufferDesc.byteSize = sizeof(ConstantBufferData);
        constantBufferDesc.isConstantBuffer = true;
        constantBufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        constantBufferDesc.keepInitialState = true;
        constantBufferDesc.debugName = "SOGE geometry entity constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(constantBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set for geometry entity...");
        nvrhi::BindingSetDesc geometryBindingSetDesc{};
        geometryBindingSetDesc.trackLiveness = true;
        geometryBindingSetDesc.bindings = {
            nvrhi::BindingSetItem::ConstantBuffer(1, m_nvrhiConstantBuffer),
        };
        m_nvrhiBindingSet = device.createBindingSet(geometryBindingSetDesc, &aPipeline.GetEntityBindingLayout());
    }

    const Transform& GeometryEntity::GetTransform() const
    {
        return m_transform;
    }

    Transform& GeometryEntity::GetTransform()
    {
        m_shouldWrite = true;
        return m_transform;
    }

    void GeometryEntity::UpdateVertices(const Vertices aVertices)
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
            SOGE_INFO_LOG("Creating NVRHI vertex buffer for geometry entity...");
            nvrhi::BufferDesc bufferDesc{};
            bufferDesc.byteSize = inputByteSize;
            bufferDesc.isVertexBuffer = true;
            bufferDesc.initialState = nvrhi::ResourceStates::VertexBuffer;
            bufferDesc.keepInitialState = true;
            bufferDesc.debugName = "SOGE geometry entity vertex buffer";
            m_nvrhiVertexBuffer = device.createBuffer(bufferDesc);
        }

        SOGE_INFO_LOG("Updating NVRHI vertex buffer for geometry entity...");
        const nvrhi::CommandListHandle updateCommandList = device.createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};
            commandList->writeBuffer(m_nvrhiVertexBuffer, aVertices.data(), inputByteSize);
        }
        core.ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    void GeometryEntity::UpdateIndices(const Indices aIndices)
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
            SOGE_INFO_LOG("Creating NVRHI index buffer for geometry entity...");
            nvrhi::BufferDesc bufferDesc{};
            bufferDesc.byteSize = inputByteSize;
            bufferDesc.isIndexBuffer = true;
            bufferDesc.initialState = nvrhi::ResourceStates::IndexBuffer;
            bufferDesc.keepInitialState = true;
            bufferDesc.debugName = "SOGE geometry entity index buffer";
            m_nvrhiIndexBuffer = device.createBuffer(bufferDesc);
        }

        SOGE_INFO_LOG("Updating NVRHI index buffer for geometry entity...");
        const nvrhi::CommandListHandle updateCommandList = device.createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};
            commandList->writeBuffer(m_nvrhiIndexBuffer, aIndices.data(), inputByteSize);
        }
        core.ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    nvrhi::BindingSetHandle GeometryEntity::GetBindingSet(Tag)
    {
        return m_nvrhiBindingSet;
    }

    nvrhi::BufferHandle GeometryEntity::GetVertexBuffer(Tag)
    {
        return m_nvrhiVertexBuffer;
    }

    nvrhi::BufferHandle GeometryEntity::GetIndexBuffer(Tag)
    {
        return m_nvrhiIndexBuffer;
    }

    void GeometryEntity::WriteConstantBuffer(Tag, nvrhi::ICommandList& aCommandList)
    {
        if (!m_shouldWrite)
        {
            return;
        }
        m_shouldWrite = false;

        SOGE_INFO_LOG("Updating NVRHI constant buffer for geometry entity...");
        const ConstantBufferData constantBufferData{
            .m_model = m_transform.WorldMatrix(),
        };
        aCommandList.writeBuffer(m_nvrhiConstantBuffer, &constantBufferData, sizeof(constantBufferData));
    }

    void GeometryEntity::WriteVertexBuffer(Tag, nvrhi::ICommandList& aCommandList)
    {
        // empty for now
    }

    void GeometryEntity::WriteIndexBuffer(Tag, nvrhi::ICommandList& aCommandList)
    {
        // empty for now
    }
}
