#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/GeometryGraphicsPipeline.hpp"


namespace soge
{
    GeometryGraphicsPipeline::GeometryGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aRenderPass,
                                                       GeometryVertexShaderResource& aVertexShader,
                                                       GeometryPixelShaderResource& aPixelShader)
        : m_core{aCore}, m_renderPass{aRenderPass}
    {
        SOGE_INFO_LOG("Creating NVRHI geometry pipeline...");
        nvrhi::IDevice& device = aCore.GetRawDevice();

        const std::array vertexAttributeDescArray{
            nvrhi::VertexAttributeDesc{
                .name = "position",
                .format = nvrhi::Format::RGB32_FLOAT,
                .offset = offsetof(Entity::Vertex, m_position),
                .elementStride = sizeof(Entity::Vertex),
            },
            nvrhi::VertexAttributeDesc{
                .name = "normal",
                .format = nvrhi::Format::RGB32_FLOAT,
                .offset = offsetof(Entity::Vertex, m_normal),
                .elementStride = sizeof(Entity::Vertex),
            },
            nvrhi::VertexAttributeDesc{
                .name = "color",
                .format = nvrhi::Format::RGBA32_FLOAT,
                .offset = offsetof(Entity::Vertex, m_color),
                .elementStride = sizeof(Entity::Vertex),
            },
        };
        m_nvrhiInputLayout = device.createInputLayout(vertexAttributeDescArray.data(),
                                                      static_cast<std::uint32_t>(vertexAttributeDescArray.size()),
                                                      &aVertexShader.GetResource());

        nvrhi::BindingLayoutDesc bindingLayoutDesc{};
        bindingLayoutDesc.visibility = nvrhi::ShaderType::Vertex;
        bindingLayoutDesc.bindings = {
            nvrhi::BindingLayoutItem::ConstantBuffer(0), // view & projection matrix
        };
        m_nvrhiBindingLayout = device.createBindingLayout(bindingLayoutDesc);

        nvrhi::BindingLayoutDesc entityBindingLayoutDesc{};
        entityBindingLayoutDesc.visibility = nvrhi::ShaderType::Vertex;
        entityBindingLayoutDesc.bindings = {
            nvrhi::BindingLayoutItem::ConstantBuffer(1), // model matrix
        };
        m_nvrhiEntityBindingLayout = device.createBindingLayout(entityBindingLayoutDesc);

        nvrhi::GraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.inputLayout = m_nvrhiInputLayout;
        pipelineDesc.VS = &aVertexShader.GetResource();
        pipelineDesc.PS = &aPixelShader.GetResource();
        pipelineDesc.bindingLayouts = {m_nvrhiBindingLayout, m_nvrhiEntityBindingLayout};
        nvrhi::IFramebuffer& compatibleFramebuffer = aRenderPass.GetFramebuffer();
        m_nvrhiGraphicsPipeline = device.createGraphicsPipeline(pipelineDesc, &compatibleFramebuffer);

        SOGE_INFO_LOG("Creating NVRHI constant buffer for geometry pipeline...");
        nvrhi::BufferDesc constantBufferDesc{};
        constantBufferDesc.byteSize = sizeof(ConstantBufferData);
        constantBufferDesc.isConstantBuffer = true;
        constantBufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        constantBufferDesc.keepInitialState = true;
        constantBufferDesc.debugName = "SOGE geometry pipeline constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(constantBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set for geometry pipeline...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        bindingSetDesc.bindings = {
            nvrhi::BindingSetItem::ConstantBuffer(0, m_nvrhiConstantBuffer),
        };
        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, m_nvrhiBindingLayout);
    }

    nvrhi::IBindingLayout& GeometryGraphicsPipeline::GetEntityBindingLayout()
    {
        return *m_nvrhiEntityBindingLayout;
    }

    void GeometryGraphicsPipeline::WriteConstantBuffer(const Camera& aCamera, nvrhi::ICommandList& aCommandList)
    {
        const ConstantBufferData constantBufferData{
            .m_viewProjection = aCamera.GetProjectionMatrix() * aCamera.m_transform.ViewMatrix(),
        };
        aCommandList.writeBuffer(m_nvrhiConstantBuffer, &constantBufferData, sizeof(constantBufferData));
    }

    nvrhi::IGraphicsPipeline& GeometryGraphicsPipeline::GetGraphicsPipeline() noexcept
    {
        return *m_nvrhiGraphicsPipeline;
    }

    void GeometryGraphicsPipeline::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                                           nvrhi::ICommandList& aCommandList)
    {
        aEntity.WriteResources({}, aCommandList);

        const auto entityBindingSet = aEntity.GetBindingSet({});
        const auto entityVertexBuffer = aEntity.GetVertexBuffer({});
        const auto entityIndexBuffer = aEntity.GetIndexBuffer({});

        nvrhi::GraphicsState graphicsState{};
        graphicsState.pipeline = &GetGraphicsPipeline();
        graphicsState.framebuffer = &m_renderPass.get().GetFramebuffer();
        graphicsState.bindings = {m_nvrhiBindingSet, entityBindingSet};
        if (entityVertexBuffer != nullptr)
        {
            const nvrhi::VertexBufferBinding vertexBufferBinding{
                .buffer = entityVertexBuffer,
                .slot = 0,
                .offset = 0,
            };
            graphicsState.addVertexBuffer(vertexBufferBinding);
        }
        if (entityIndexBuffer != nullptr)
        {
            graphicsState.indexBuffer = nvrhi::IndexBufferBinding{
                .buffer = entityIndexBuffer,
                .format = nvrhi::Format::R32_UINT,
                .offset = 0,
            };
        }
        graphicsState.viewport.addViewportAndScissorRect(aViewport);
        aCommandList.setGraphicsState(graphicsState);

        nvrhi::DrawArguments drawArguments{};
        if (entityIndexBuffer != nullptr)
        {
            const auto& desc = entityIndexBuffer->getDesc();
            drawArguments.vertexCount = static_cast<std::uint32_t>(desc.byteSize / sizeof(Entity::Index));

            aCommandList.drawIndexed(drawArguments);
        }
        else if (entityVertexBuffer != nullptr)
        {
            const auto& desc = entityVertexBuffer->getDesc();
            drawArguments.vertexCount = static_cast<std::uint32_t>(desc.byteSize / sizeof(Entity::Vertex));

            aCommandList.draw(drawArguments);
        }
    }
}
