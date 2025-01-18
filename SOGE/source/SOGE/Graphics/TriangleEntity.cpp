#include "sogepch.hpp"

#include "SOGE/Graphics/TriangleEntity.hpp"

#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"


namespace soge
{
    TriangleEntity::TriangleEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline, Transform aTransform)
        : m_core{aCore}, m_pipeline{aPipeline}, m_transform{aTransform}
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

        SOGE_INFO_LOG("Creating NVRHI binding set for triangle entity...");
        nvrhi::BindingSetDesc geometryBindingSetDesc{};
        geometryBindingSetDesc.trackLiveness = true;
        geometryBindingSetDesc.bindings = {
            nvrhi::BindingSetItem::ConstantBuffer(1, m_nvrhiConstantBuffer),
        };
        m_nvrhiBindingSet = device.createBindingSet(geometryBindingSetDesc, &aPipeline.GetEntityBindingLayout());

        SOGE_INFO_LOG("Updating NVRHI constant buffer for triangle entity...");
        const nvrhi::CommandListHandle updateCommandList = m_core.get().GetRawDevice().createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            const ConstantBuffer constantBuffer{
                .m_model = m_transform.WorldMatrix(),
            };
            commandList->writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    Transform TriangleEntity::GetTransform() const
    {
        return m_transform;
    }

    void TriangleEntity::SetTransform(const Transform& aTransform)
    {
        if (m_transform == aTransform)
        {
            return;
        }
        m_transform = aTransform;

        SOGE_INFO_LOG("Updating NVRHI constant buffer for triangle entity...");
        const nvrhi::CommandListHandle updateCommandList = m_core.get().GetRawDevice().createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            const ConstantBuffer constantBuffer{
                .m_model = m_transform.WorldMatrix(),
            };
            commandList->writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
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

    nvrhi::BindingSetHandle TriangleEntity::GetBindingSet(Tag)
    {
        return m_nvrhiBindingSet;
    }

    nvrhi::BufferHandle TriangleEntity::GetConstantBuffer(Tag)
    {
        return m_nvrhiConstantBuffer;
    }

    nvrhi::BufferHandle TriangleEntity::GetVertexBuffer(Tag)
    {
        return m_nvrhiVertexBuffer;
    }

    nvrhi::BufferHandle TriangleEntity::GetIndexBuffer(Tag)
    {
        return m_nvrhiIndexBuffer;
    }
}
