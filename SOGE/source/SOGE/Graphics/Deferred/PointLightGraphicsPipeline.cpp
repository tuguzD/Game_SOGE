#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/PointLightGraphicsPipeline.hpp"


namespace soge
{
    PointLightGraphicsPipeline::PointLightGraphicsPipeline(GraphicsCore& aCore,
                                                           GeometryGraphicsRenderPass& aGeometryRenderPass,
                                                           FinalGraphicsRenderPass& aFinalRenderPass,
                                                           PointLightVertexShaderResource& aVertexShader,
                                                           PointLightPixelShaderResource& aPixelShader)
        : m_core{aCore}, m_geometryRenderPass{aGeometryRenderPass}, m_finalRenderPass{aFinalRenderPass}
    {
        SOGE_INFO_LOG("Creating NVRHI point light pipeline...");
        nvrhi::IDevice& device = aCore.GetRawDevice();

        const std::array vertexAttributeDescArray{
            nvrhi::VertexAttributeDesc{
                .name = "position",
                .format = nvrhi::Format::RGB32_FLOAT,
                .offset = offsetof(Entity::Vertex, m_position),
                .elementStride = sizeof(Entity::Vertex),
            },
        };
        m_nvrhiInputLayout = device.createInputLayout(vertexAttributeDescArray.data(),
                                                      static_cast<std::uint32_t>(vertexAttributeDescArray.size()),
                                                      aVertexShader.GetResource());

        nvrhi::BindingLayoutDesc bindingLayoutDesc{};
        bindingLayoutDesc.visibility = nvrhi::ShaderType::All;
        bindingLayoutDesc.bindings = {
            nvrhi::BindingLayoutItem::ConstantBuffer(0), // inverse view & projection
            nvrhi::BindingLayoutItem::Texture_SRV(0),    // depth
            nvrhi::BindingLayoutItem::Texture_SRV(1),    // albedo
            nvrhi::BindingLayoutItem::Texture_SRV(2),    // normal
        };
        m_nvrhiBindingLayout = device.createBindingLayout(bindingLayoutDesc);

        nvrhi::BindingLayoutDesc entityBindingLayoutDesc{};
        entityBindingLayoutDesc.visibility = nvrhi::ShaderType::All;
        entityBindingLayoutDesc.bindings = {
            nvrhi::BindingLayoutItem::ConstantBuffer(1), // point light
        };
        m_nvrhiEntityBindingLayout = device.createBindingLayout(entityBindingLayoutDesc);

        nvrhi::GraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.inputLayout = m_nvrhiInputLayout;
        pipelineDesc.VS = aVertexShader.GetResource();
        pipelineDesc.PS = aPixelShader.GetResource();
        pipelineDesc.bindingLayouts = {m_nvrhiBindingLayout, m_nvrhiEntityBindingLayout};
        pipelineDesc.renderState.depthStencilState.depthTestEnable = false;
        pipelineDesc.renderState.depthStencilState.depthWriteEnable = false;
        pipelineDesc.renderState.rasterState.frontCounterClockwise = true;

        nvrhi::BlendState::RenderTarget blendDesc{};
        blendDesc.blendEnable = true;
        blendDesc.srcBlend = nvrhi::BlendFactor::One;
        blendDesc.destBlend = nvrhi::BlendFactor::One;
        blendDesc.blendOp = nvrhi::BlendOp::Add;
        pipelineDesc.renderState.blendState.setRenderTarget(0, blendDesc);

        // no need to create pipeline for each frame buffer, all of them are compatible with the first one
        nvrhi::IFramebuffer& compatibleFramebuffer = aFinalRenderPass.GetFramebuffer();
        m_nvrhiGraphicsPipeline = device.createGraphicsPipeline(pipelineDesc, &compatibleFramebuffer);

        SOGE_INFO_LOG("Creating NVRHI constant buffer for point light pipeline...");
        nvrhi::BufferDesc constantBufferDesc{};
        constantBufferDesc.byteSize = sizeof(ConstantBufferData);
        constantBufferDesc.isConstantBuffer = true;
        constantBufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        constantBufferDesc.keepInitialState = true;
        constantBufferDesc.debugName = "SOGE point light pipeline constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(constantBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set for point light pipeline...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;

        const auto depthTexture = aGeometryRenderPass.GetFramebuffer().getDesc().depthAttachment.texture;
        bindingSetDesc.bindings = {
            nvrhi::BindingSetItem::ConstantBuffer(0, m_nvrhiConstantBuffer),
            nvrhi::BindingSetItem::Texture_SRV(0, depthTexture),
            nvrhi::BindingSetItem::Texture_SRV(1, &aGeometryRenderPass.GetAlbedoTexture()),
            nvrhi::BindingSetItem::Texture_SRV(2, &aGeometryRenderPass.GetNormalTexture()),
        };

        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, m_nvrhiBindingLayout);
    }

    nvrhi::IBindingLayout& PointLightGraphicsPipeline::GetEntityBindingLayout()
    {
        return *m_nvrhiEntityBindingLayout;
    }

    void PointLightGraphicsPipeline::WriteConstantBuffer(const Camera& aCamera, nvrhi::ICommandList& aCommandList)
    {
        const ConstantBufferData constantBuffer{
            .m_viewProjection = aCamera.GetProjectionMatrix() * aCamera.m_transform.ViewMatrix(),
            .m_invProjection = glm::inverse(aCamera.GetProjectionMatrix()),
            .m_invView = glm::inverse(aCamera.m_transform.ViewMatrix()),
            .m_viewPosition = aCamera.m_transform.m_position,
        };
        aCommandList.writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
    }

    nvrhi::IGraphicsPipeline& PointLightGraphicsPipeline::GetGraphicsPipeline()
    {
        return *m_nvrhiGraphicsPipeline;
    }

    void PointLightGraphicsPipeline::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                                             nvrhi::ICommandList& aCommandList)
    {
        aEntity.WriteResources({}, aCommandList);

        const auto entityBindingSet = aEntity.GetBindingSet({});
        const auto entityVertexBuffer = aEntity.GetVertexBuffer({});
        const auto entityIndexBuffer = aEntity.GetIndexBuffer({});

        nvrhi::GraphicsState graphicsState{};
        graphicsState.pipeline = &GetGraphicsPipeline();
        graphicsState.framebuffer = &m_finalRenderPass.get().GetFramebuffer();
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
