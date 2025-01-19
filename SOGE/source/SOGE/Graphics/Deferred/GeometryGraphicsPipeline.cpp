#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/GeometryGraphicsPipeline.hpp"

#include "SOGE/Graphics/Utils/LoadShader.hpp"


namespace soge
{
    GeometryGraphicsPipeline::GeometryGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aRenderPass)
        : m_core{aCore}, m_renderPass{aRenderPass}
    {
        SOGE_INFO_LOG("Creating NVRHI geometry pipeline...");
        nvrhi::IDevice& device = aCore.GetRawDevice();

        constexpr auto shaderSourcePath = "./resources/shaders/deferred_geometry.hlsl";

        nvrhi::ShaderDesc vertexShaderDesc{};
        vertexShaderDesc.shaderType = nvrhi::ShaderType::Vertex;
        vertexShaderDesc.debugName = "SOGE geometry pipeline vertex shader";
        vertexShaderDesc.entryName = "VSMain";
        m_nvrhiVertexShader = LoadShader(aCore, vertexShaderDesc, shaderSourcePath, "VSMain");

        nvrhi::ShaderDesc pixelShaderDesc{};
        pixelShaderDesc.shaderType = nvrhi::ShaderType::Pixel;
        pixelShaderDesc.debugName = "SOGE geometry pipeline pixel shader";
        pixelShaderDesc.entryName = "PSMain";
        m_nvrhiPixelShader = LoadShader(aCore, pixelShaderDesc, shaderSourcePath, "PSMain");

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
        m_nvrhiInputLayout =
            device.createInputLayout(vertexAttributeDescArray.data(),
                                     static_cast<std::uint32_t>(vertexAttributeDescArray.size()), m_nvrhiVertexShader);

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
        pipelineDesc.VS = m_nvrhiVertexShader;
        pipelineDesc.PS = m_nvrhiPixelShader;
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
        aEntity.WriteConstantBuffer({}, aCommandList);
        aEntity.WriteVertexBuffer({}, aCommandList);
        aEntity.WriteIndexBuffer({}, aCommandList);

        const auto vertexBuffer = aEntity.GetVertexBuffer({});
        const auto indexBuffer = aEntity.GetIndexBuffer({});

        nvrhi::GraphicsState graphicsState{};
        graphicsState.pipeline = &GetGraphicsPipeline();
        graphicsState.framebuffer = &m_renderPass.get().GetFramebuffer();
        graphicsState.bindings = {m_nvrhiBindingSet, aEntity.GetBindingSet({})};
        if (vertexBuffer != nullptr)
        {
            const nvrhi::VertexBufferBinding vertexBufferBinding{
                .buffer = vertexBuffer,
                .slot = 0,
                .offset = 0,
            };
            graphicsState.addVertexBuffer(vertexBufferBinding);
        }
        if (indexBuffer != nullptr)
        {
            graphicsState.indexBuffer = nvrhi::IndexBufferBinding{
                .buffer = indexBuffer,
                .format = nvrhi::Format::R32_UINT,
                .offset = 0,
            };
        }
        graphicsState.viewport.addViewportAndScissorRect(aViewport);
        aCommandList.setGraphicsState(graphicsState);

        nvrhi::DrawArguments drawArguments{};
        if (indexBuffer != nullptr)
        {
            const auto& desc = indexBuffer->getDesc();
            drawArguments.vertexCount = static_cast<std::uint32_t>(desc.byteSize / sizeof(Entity::Index));

            aCommandList.drawIndexed(drawArguments);
        }
        else if (vertexBuffer != nullptr)
        {
            const auto& desc = vertexBuffer->getDesc();
            drawArguments.vertexCount = static_cast<std::uint32_t>(desc.byteSize / sizeof(Entity::Vertex));

            aCommandList.draw(drawArguments);
        }
    }
}
