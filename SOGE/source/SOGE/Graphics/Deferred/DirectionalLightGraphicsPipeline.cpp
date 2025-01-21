#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/DirectionalLightGraphicsPipeline.hpp"

#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"


namespace soge
{
    DirectionalLightGraphicsPipeline::DirectionalLightGraphicsPipeline(
        GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryRenderPass, FinalGraphicsRenderPass& aFinalRenderPass,
        DirectionalLightVertexShaderResource& aVertexShader, DirectionalLightPixelShaderResource& aPixelShader)
        : m_core{aCore}, m_geometryRenderPass{aGeometryRenderPass}, m_finalRenderPass{aFinalRenderPass}
    {
        SOGE_INFO_LOG("Creating NVRHI directional light pipeline...");
        nvrhi::IDevice& device = aCore.GetRawDevice();

        const std::array vertexAttributeDescArray{
            nvrhi::VertexAttributeDesc{
                .name = "position",
                .format = nvrhi::Format::RGB32_FLOAT,
                .offset = 0,
                .elementStride = sizeof(glm::vec3),
            },
        };
        m_nvrhiInputLayout = device.createInputLayout(vertexAttributeDescArray.data(),
                                                      static_cast<std::uint32_t>(vertexAttributeDescArray.size()),
                                                      aVertexShader.GetShaderResource());

        nvrhi::BindingLayoutDesc bindingLayoutDesc{};
        bindingLayoutDesc.visibility = nvrhi::ShaderType::Pixel;
        bindingLayoutDesc.bindings = {
            nvrhi::BindingLayoutItem::ConstantBuffer(0), // inverse view & projection
            nvrhi::BindingLayoutItem::Texture_SRV(0),    // depth
            nvrhi::BindingLayoutItem::Texture_SRV(1),    // albedo
            nvrhi::BindingLayoutItem::Texture_SRV(2),    // normal
            nvrhi::BindingLayoutItem::Texture_SRV(3),    // diffuse
            nvrhi::BindingLayoutItem::Texture_SRV(4),    // specularShininess
        };
        m_nvrhiBindingLayout = device.createBindingLayout(bindingLayoutDesc);

        nvrhi::BindingLayoutDesc entityBindingLayoutDesc{};
        entityBindingLayoutDesc.visibility = nvrhi::ShaderType::Pixel;
        entityBindingLayoutDesc.bindings = {
            nvrhi::BindingLayoutItem::ConstantBuffer(1), // directional light
        };
        m_nvrhiEntityBindingLayout = device.createBindingLayout(entityBindingLayoutDesc);

        nvrhi::GraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.inputLayout = m_nvrhiInputLayout;
        pipelineDesc.VS = aVertexShader.GetShaderResource();
        pipelineDesc.PS = aPixelShader.GetShaderResource();
        pipelineDesc.bindingLayouts = {m_nvrhiBindingLayout, m_nvrhiEntityBindingLayout};
        pipelineDesc.renderState.depthStencilState.depthTestEnable = false;
        pipelineDesc.renderState.depthStencilState.depthWriteEnable = false;

        nvrhi::BlendState::RenderTarget blendDesc{};
        blendDesc.blendEnable = true;
        blendDesc.srcBlend = nvrhi::BlendFactor::One;
        blendDesc.destBlend = nvrhi::BlendFactor::One;
        blendDesc.blendOp = nvrhi::BlendOp::Add;
        pipelineDesc.renderState.blendState.setRenderTarget(0, blendDesc);

        // no need to create pipeline for each frame buffer, all of them are compatible with the first one
        nvrhi::IFramebuffer& compatibleFramebuffer = aFinalRenderPass.GetFramebuffer();
        m_nvrhiGraphicsPipeline = device.createGraphicsPipeline(pipelineDesc, &compatibleFramebuffer);

        SOGE_INFO_LOG("Creating NVRHI constant buffer for directional light pipeline...");
        nvrhi::BufferDesc constantBufferDesc{};
        constantBufferDesc.byteSize = sizeof(ConstantBufferData);
        constantBufferDesc.isConstantBuffer = true;
        constantBufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        constantBufferDesc.keepInitialState = true;
        constantBufferDesc.debugName = "SOGE directional light pipeline constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(constantBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI vertex buffer for directional light pipeline...");
        nvrhi::BufferDesc vertexBufferDesc{};
        vertexBufferDesc.byteSize = sizeof(glm::vec3) * 4;
        vertexBufferDesc.isVertexBuffer = true;
        vertexBufferDesc.initialState = nvrhi::ResourceStates::VertexBuffer;
        vertexBufferDesc.keepInitialState = true;
        vertexBufferDesc.debugName = "SOGE directional light pipeline vertex buffer";
        m_nvrhiVertexBuffer = device.createBuffer(vertexBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI index buffer for directional light pipeline...");
        nvrhi::BufferDesc indexBufferDesc{};
        indexBufferDesc.byteSize = sizeof(std::uint32_t) * 6;
        indexBufferDesc.isIndexBuffer = true;
        indexBufferDesc.initialState = nvrhi::ResourceStates::IndexBuffer;
        indexBufferDesc.keepInitialState = true;
        indexBufferDesc.debugName = "SOGE directional light pipeline index buffer";
        m_nvrhiIndexBuffer = device.createBuffer(indexBufferDesc);

        SOGE_INFO_LOG("Updating NVRHI vertex & index buffer for directional light pipeline...");
        const nvrhi::CommandListHandle updateCommandList = device.createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            constexpr std::array vertices{
                glm::vec3{1.0f, 1.0f, 0.0f},
                glm::vec3{1.0f, -1.0f, 0.0f},
                glm::vec3{-1.0f, 1.0f, 0.0f},
                glm::vec3{-1.0f, -1.0f, 0.0f},
            };
            commandList->writeBuffer(m_nvrhiVertexBuffer, vertices.data(), vertexBufferDesc.byteSize);

            constexpr std::array indices{0u, 1u, 2u, 3u, 2u, 1u};
            commandList->writeBuffer(m_nvrhiIndexBuffer, indices.data(), indexBufferDesc.byteSize);
        }
        aCore.ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);

        SOGE_INFO_LOG("Creating NVRHI binding set for directional light pipeline...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;

        const auto depthTexture = aGeometryRenderPass.GetFramebuffer().getDesc().depthAttachment.texture;
        bindingSetDesc.bindings = {
            nvrhi::BindingSetItem::ConstantBuffer(0, m_nvrhiConstantBuffer),
            nvrhi::BindingSetItem::Texture_SRV(0, depthTexture),
            nvrhi::BindingSetItem::Texture_SRV(1, &aGeometryRenderPass.GetAlbedoTexture()),
            nvrhi::BindingSetItem::Texture_SRV(2, &aGeometryRenderPass.GetNormalTexture()),
            nvrhi::BindingSetItem::Texture_SRV(3, &aGeometryRenderPass.GetDiffuseTexture()),
            nvrhi::BindingSetItem::Texture_SRV(4, &aGeometryRenderPass.GetSpecularShininessTexture()),
        };

        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, m_nvrhiBindingLayout);
    }

    nvrhi::IBindingLayout& DirectionalLightGraphicsPipeline::GetEntityBindingLayout()
    {
        return *m_nvrhiEntityBindingLayout;
    }

    void DirectionalLightGraphicsPipeline::WriteConstantBuffer(const Camera& aCamera, nvrhi::ICommandList& aCommandList)
    {
        const ConstantBufferData constantBuffer{
            .m_invProjection = glm::inverse(aCamera.GetProjectionMatrix()),
            .m_invView = glm::inverse(aCamera.m_transform.ViewMatrix()),
            .m_viewPosition = aCamera.m_transform.m_position,
        };
        aCommandList.writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
    }

    nvrhi::IGraphicsPipeline& DirectionalLightGraphicsPipeline::GetGraphicsPipeline()
    {
        return *m_nvrhiGraphicsPipeline;
    }

    void DirectionalLightGraphicsPipeline::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera,
                                                   Entity& aEntity, nvrhi::ICommandList& aCommandList)
    {
        const auto entityBindingSet = aEntity.GetBindingSet({});

        nvrhi::GraphicsState graphicsState{};
        graphicsState.pipeline = &GetGraphicsPipeline();
        graphicsState.framebuffer = &m_finalRenderPass.get().GetFramebuffer();
        graphicsState.bindings = {m_nvrhiBindingSet, entityBindingSet};
        graphicsState.addVertexBuffer(nvrhi::VertexBufferBinding{
            .buffer = m_nvrhiVertexBuffer,
            .slot = 0,
            .offset = 0,
        });
        graphicsState.indexBuffer = nvrhi::IndexBufferBinding{
            .buffer = m_nvrhiIndexBuffer,
            .format = nvrhi::Format::R32_UINT,
            .offset = 0,
        };
        graphicsState.viewport.addViewportAndScissorRect(aViewport);
        aCommandList.setGraphicsState(graphicsState);

        nvrhi::DrawArguments drawArguments{};
        const auto& desc = m_nvrhiIndexBuffer->getDesc();
        drawArguments.vertexCount = static_cast<std::uint32_t>(desc.byteSize / sizeof(std::uint32_t));

        aCommandList.drawIndexed(drawArguments);
    }
}
