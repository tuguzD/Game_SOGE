#include "sogepch.hpp"

#include "SOGE/Graphics/TriangleEntity.hpp"

#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"


namespace soge
{
    TriangleEntity::TriangleEntity(GraphicsCore& aCore, TriangleGraphicsPipeline& aPipeline)
        : m_core{aCore}, m_pipeline{aPipeline}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        SOGE_INFO_LOG("Creating NVRHI constant buffer for triangle entity...");
        nvrhi::BufferDesc bufferDesc{};
        bufferDesc.byteSize = sizeof(glm::mat4x4);
        bufferDesc.isConstantBuffer = true;
        bufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        bufferDesc.keepInitialState = true;
        bufferDesc.debugName = "SOGE triangle entity constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(bufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set for triangle entity...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        bindingSetDesc.addItem(nvrhi::BindingSetItem::ConstantBuffer(0, m_nvrhiConstantBuffer));

        const auto bindingLayout = aPipeline.GetGraphicsPipeline().getDesc().bindingLayouts[0];
        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, bindingLayout);
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

    nvrhi::CommandListHandle TriangleEntity::Update(const nvrhi::Viewport& aViewport, const Camera& aCamera,
                                                    GraphicsRenderPass& aRenderPass, GraphicsPipeline& aPipeline)
    {
        // check if render passes are the same
        if (&aRenderPass != &m_pipeline.get().GetRenderPass())
        {
            return {};
        }

        // check for pipeline framebuffer compatibility
        if (aPipeline.GetGraphicsPipeline().getFramebufferInfo() !=
            m_pipeline.get().GetGraphicsPipeline().getFramebufferInfo())
        {
            return {};
        }

        nvrhi::CommandListParameters commandListDesc{};
        commandListDesc.enableImmediateExecution = false;

        nvrhi::IDevice& device = m_core.get().GetRawDevice();
        const nvrhi::CommandListHandle drawCommandList = device.createCommandList(commandListDesc);
        {
            GraphicsCommandListGuard commandList{*drawCommandList};

            const glm::mat4x4 mvp =
                aCamera.GetProjectionMatrix() * aCamera.m_transform.ViewMatrix() * m_transform.WorldMatrix();
            commandList->writeBuffer(m_nvrhiConstantBuffer, &mvp, sizeof(glm::mat4x4));

            nvrhi::GraphicsState graphicsState{};
            graphicsState.pipeline = &aPipeline.GetGraphicsPipeline();
            graphicsState.framebuffer = &aRenderPass.GetFramebuffer();
            graphicsState.bindings = {m_nvrhiBindingSet};
            if (m_nvrhiVertexBuffer != nullptr)
            {
                const nvrhi::VertexBufferBinding vertexBufferBinding{
                    .buffer = m_nvrhiVertexBuffer,
                    .slot = 0,
                    .offset = 0,
                };
                graphicsState.addVertexBuffer(vertexBufferBinding);
            }
            if (m_nvrhiIndexBuffer != nullptr)
            {
                graphicsState.indexBuffer = nvrhi::IndexBufferBinding{
                    .buffer = m_nvrhiIndexBuffer,
                    .format = nvrhi::Format::R32_UINT,
                    .offset = 0,
                };
            }
            graphicsState.viewport.addViewportAndScissorRect(aViewport);
            commandList->setGraphicsState(graphicsState);

            nvrhi::DrawArguments drawArguments{};
            if (m_nvrhiIndexBuffer != nullptr)
            {
                const auto& desc = m_nvrhiIndexBuffer->getDesc();
                drawArguments.vertexCount = static_cast<std::uint32_t>(desc.byteSize / sizeof(Index));

                commandList->drawIndexed(drawArguments);
            }
            else if (m_nvrhiVertexBuffer != nullptr)
            {
                const auto& desc = m_nvrhiVertexBuffer->getDesc();
                drawArguments.vertexCount = static_cast<std::uint32_t>(desc.byteSize / sizeof(Vertex));

                commandList->draw(drawArguments);
            }
        }

        return drawCommandList;
    }
}
