#include "sogepch.hpp"

#include "SOGE/Graphics/TriangleEntity.hpp"

#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"


namespace soge
{
    TriangleEntity::TriangleEntity(GraphicsCore& aCore, TriangleGraphicsPipeline& aPipeline, const Vertices aVertices)
        : m_core{aCore}, m_pipeline{aPipeline}
    {
        SOGE_INFO_LOG("Creating NVRHI binding set...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        m_nvrhiBindingSet = aCore.GetRawDevice().createBindingSet(bindingSetDesc, &aPipeline.GetBindingLayout());

        UpdateVertices(aVertices);
    }

    void TriangleEntity::UpdateVertices(const Vertices aVertices)
    {
        if (aVertices.empty())
        {
            m_nvrhiVertexBuffer = nullptr;
            return;
        }

        nvrhi::IDevice& device = m_core.get().GetRawDevice();
        const auto inputByteSize = static_cast<std::uint64_t>(aVertices.size_bytes());

        if (m_nvrhiVertexBuffer == nullptr || m_nvrhiVertexBuffer->getDesc().byteSize != inputByteSize)
        {
            SOGE_INFO_LOG("Creating NVRHI vertex buffer...");
            nvrhi::BufferDesc vertexBufferDesc{};
            vertexBufferDesc.byteSize = inputByteSize;
            vertexBufferDesc.isVertexBuffer = true;
            vertexBufferDesc.initialState = nvrhi::ResourceStates::VertexBuffer;
            vertexBufferDesc.keepInitialState = true;
            vertexBufferDesc.debugName = "SOGE vertex buffer";
            m_nvrhiVertexBuffer = device.createBuffer(vertexBufferDesc);
        }

        SOGE_INFO_LOG("Updating NVRHI vertex buffer...");
        const nvrhi::CommandListHandle verticesCommandList = device.createCommandList();
        {
            GraphicsCommandListGuard commandList{*verticesCommandList};
            commandList->writeBuffer(m_nvrhiVertexBuffer, aVertices.data(), inputByteSize);
        }
        device.executeCommandList(verticesCommandList, nvrhi::CommandQueue::Graphics);
    }

    nvrhi::CommandListHandle TriangleEntity::Update(GraphicsRenderPass& aRenderPass, GraphicsPipeline& aPipeline)
    {
        if (m_nvrhiVertexBuffer == nullptr)
        {
            return {};
        }

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
        const nvrhi::CommandListHandle triangleCommandList = device.createCommandList(commandListDesc);
        {
            GraphicsCommandListGuard commandList{*triangleCommandList};

            nvrhi::IFramebuffer& currentFramebuffer = aRenderPass.GetFramebuffer();
            const nvrhi::FramebufferInfoEx& framebufferInfo = currentFramebuffer.getFramebufferInfo();

            nvrhi::GraphicsState graphicsState{};
            graphicsState.pipeline = &aPipeline.GetGraphicsPipeline();
            graphicsState.framebuffer = &currentFramebuffer;
            graphicsState.bindings = {m_nvrhiBindingSet};
            graphicsState.vertexBuffers = {
                nvrhi::VertexBufferBinding{.buffer = m_nvrhiVertexBuffer, .slot = 0, .offset = 0},
            };
            graphicsState.viewport.addViewportAndScissorRect(framebufferInfo.getViewport());
            commandList->setGraphicsState(graphicsState);

            const auto& vertexBufferDesc = m_nvrhiVertexBuffer->getDesc();
            const auto vertexCount = static_cast<std::uint32_t>(vertexBufferDesc.byteSize / sizeof(Vertex));

            nvrhi::DrawArguments drawArguments{};
            drawArguments.vertexCount = vertexCount;
            commandList->draw(drawArguments);
        }

        return triangleCommandList;
    }
}
