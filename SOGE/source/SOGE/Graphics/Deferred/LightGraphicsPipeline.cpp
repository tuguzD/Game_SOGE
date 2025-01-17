#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/LightGraphicsPipeline.hpp"

#include "SOGE/Graphics/Utils/GetCompiledShaderPath.hpp"
#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"

#include <fstream>


namespace
{
    nvrhi::ShaderHandle LoadShader(soge::GraphicsCore& aCore, const nvrhi::ShaderDesc& aDesc,
                                   const std::filesystem::path& aSourcePath, const eastl::string_view aEntryName)
    {
        const auto compiledPath = soge::GetCompiledShaderPath(aCore, aSourcePath, aEntryName);
        if (!std::filesystem::exists(compiledPath))
        {
            const auto errorMessage = fmt::format(R"(Shader file "{}" does not exist)", compiledPath.generic_string());
            throw std::runtime_error{errorMessage};
        }

        std::ifstream shaderFile{compiledPath, std::ios::in | std::ios::binary};
        const std::vector<std::uint8_t> shaderBinary{std::istreambuf_iterator{shaderFile}, {}};
        return aCore.GetRawDevice().createShader(aDesc, shaderBinary.data(), shaderBinary.size());
    }
}

namespace soge
{
    LightGraphicsPipeline::LightGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryRenderPass,
                                                 FinalGraphicsRenderPass& aFinalRenderPass)
        : m_core{aCore}, m_geometryRenderPass{aGeometryRenderPass}, m_finalRenderPass{aFinalRenderPass}
    {
        SOGE_INFO_LOG("Creating NVRHI light pipeline...");
        nvrhi::IDevice& device = aCore.GetRawDevice();

        nvrhi::ShaderDesc vertexShaderDesc{};
        vertexShaderDesc.shaderType = nvrhi::ShaderType::Vertex;
        vertexShaderDesc.debugName = "SOGE light pipeline vertex shader";
        vertexShaderDesc.entryName = "VSMain";
        m_nvrhiVertexShader = LoadShader(aCore, vertexShaderDesc, "./resources/shaders/deferred_light.hlsl", "VSMain");

        nvrhi::ShaderDesc pixelShaderDesc{};
        pixelShaderDesc.shaderType = nvrhi::ShaderType::Pixel;
        pixelShaderDesc.debugName = "SOGE light pipeline pixel shader";
        pixelShaderDesc.entryName = "PSMain";
        m_nvrhiPixelShader = LoadShader(aCore, pixelShaderDesc, "./resources/shaders/deferred_light.hlsl", "PSMain");

        const std::array vertexAttributeDescArray{
            nvrhi::VertexAttributeDesc{
                .name = "position",
                .format = nvrhi::Format::RGB32_FLOAT,
                .offset = 0,
                .elementStride = sizeof(glm::vec3),
            },
        };
        m_nvrhiInputLayout =
            device.createInputLayout(vertexAttributeDescArray.data(),
                                     static_cast<std::uint32_t>(vertexAttributeDescArray.size()), m_nvrhiVertexShader);

        nvrhi::BindingLayoutDesc bindingLayoutDesc{};
        bindingLayoutDesc.visibility = nvrhi::ShaderType::All;
        bindingLayoutDesc.addItem(nvrhi::BindingLayoutItem::Texture_SRV(0));
        bindingLayoutDesc.addItem(nvrhi::BindingLayoutItem::Texture_SRV(1));
        m_nvrhiBindingLayout = device.createBindingLayout(bindingLayoutDesc);

        nvrhi::GraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.inputLayout = m_nvrhiInputLayout;
        pipelineDesc.VS = m_nvrhiVertexShader;
        pipelineDesc.PS = m_nvrhiPixelShader;
        pipelineDesc.bindingLayouts = {m_nvrhiBindingLayout};
        pipelineDesc.renderState.depthStencilState.depthTestEnable = false;
        pipelineDesc.renderState.depthStencilState.depthWriteEnable = false;
        // no need to create pipeline for each frame buffer, all of them are compatible with the first one
        nvrhi::IFramebuffer& compatibleFramebuffer = aFinalRenderPass.GetFramebuffer();
        m_nvrhiGraphicsPipeline = device.createGraphicsPipeline(pipelineDesc, &compatibleFramebuffer);

        SOGE_INFO_LOG("Creating NVRHI vertex buffer for light pipeline...");
        nvrhi::BufferDesc vertexBufferDesc{};
        vertexBufferDesc.byteSize = sizeof(glm::vec3) * 4;
        vertexBufferDesc.isVertexBuffer = true;
        vertexBufferDesc.initialState = nvrhi::ResourceStates::VertexBuffer;
        vertexBufferDesc.keepInitialState = true;
        vertexBufferDesc.debugName = "SOGE light pipeline vertex buffer";
        m_nvrhiVertexBuffer = device.createBuffer(vertexBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI index buffer for light pipeline...");
        nvrhi::BufferDesc indexBufferDesc{};
        indexBufferDesc.byteSize = sizeof(std::uint32_t) * 6;
        indexBufferDesc.isIndexBuffer = true;
        indexBufferDesc.initialState = nvrhi::ResourceStates::IndexBuffer;
        indexBufferDesc.keepInitialState = true;
        indexBufferDesc.debugName = "SOGE light pipeline index buffer";
        m_nvrhiIndexBuffer = device.createBuffer(indexBufferDesc);

        SOGE_INFO_LOG("Updating NVRHI vertex & index buffer for light pipeline...");
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

        SOGE_INFO_LOG("Creating NVRHI binding set for light pipeline...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        bindingSetDesc.addItem(nvrhi::BindingSetItem::Texture_SRV(
            0, aGeometryRenderPass.GetFramebuffer().getDesc().depthAttachment.texture));
        bindingSetDesc.addItem(nvrhi::BindingSetItem::Texture_SRV(1, &aGeometryRenderPass.GetAlbedoTexture()));

        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, m_nvrhiBindingLayout);
    }

    LightGraphicsPipeline::LightGraphicsPipeline(LightGraphicsPipeline&& aOther) noexcept
        : m_core{aOther.m_core}, m_geometryRenderPass{aOther.m_geometryRenderPass},
          m_finalRenderPass{aOther.m_finalRenderPass}
    {
        swap(aOther);
    }

    LightGraphicsPipeline& LightGraphicsPipeline::operator=(LightGraphicsPipeline&& aOther) noexcept
    {
        swap(aOther);
        return *this;
    }

    LightGraphicsPipeline::~LightGraphicsPipeline()
    {
        m_nvrhiBindingSet = nullptr;
        m_nvrhiIndexBuffer = nullptr;
        m_nvrhiVertexBuffer = nullptr;

        SOGE_INFO_LOG("Destroying NVRHI light pipeline...");
        m_nvrhiGraphicsPipeline = nullptr;
        m_nvrhiBindingLayout = nullptr;
        m_nvrhiPixelShader = nullptr;
        m_nvrhiInputLayout = nullptr;
        m_nvrhiVertexShader = nullptr;
    }

    void LightGraphicsPipeline::swap(LightGraphicsPipeline& aOther) noexcept
    {
        using std::swap;

        eastl::swap(m_core, aOther.m_core);
        eastl::swap(m_geometryRenderPass, aOther.m_geometryRenderPass);
        eastl::swap(m_finalRenderPass, aOther.m_finalRenderPass);

        swap(m_nvrhiVertexShader, aOther.m_nvrhiVertexShader);
        swap(m_nvrhiInputLayout, aOther.m_nvrhiInputLayout);
        swap(m_nvrhiPixelShader, aOther.m_nvrhiPixelShader);
        swap(m_nvrhiBindingLayout, aOther.m_nvrhiBindingLayout);
        swap(m_nvrhiGraphicsPipeline, aOther.m_nvrhiGraphicsPipeline);

        swap(m_nvrhiVertexBuffer, aOther.m_nvrhiVertexBuffer);
        swap(m_nvrhiIndexBuffer, aOther.m_nvrhiIndexBuffer);
        swap(m_nvrhiBindingSet, aOther.m_nvrhiBindingSet);
    }

    nvrhi::IGraphicsPipeline& LightGraphicsPipeline::GetGraphicsPipeline()
    {
        return *m_nvrhiGraphicsPipeline;
    }

    void LightGraphicsPipeline::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                                        nvrhi::ICommandList& aCommandList)
    {
        nvrhi::GraphicsState graphicsState{};
        graphicsState.pipeline = &GetGraphicsPipeline();
        graphicsState.framebuffer = &m_finalRenderPass.get().GetFramebuffer();
        graphicsState.bindings = {m_nvrhiBindingSet};

        const nvrhi::VertexBufferBinding vertexBufferBinding{
            .buffer = m_nvrhiVertexBuffer,
            .slot = 0,
            .offset = 0,
        };
        graphicsState.addVertexBuffer(vertexBufferBinding);

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
