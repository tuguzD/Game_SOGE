#include "sogepch.hpp"

#include "SOGE/Graphics/TriangleGraphicsPipeline.hpp"

#include "SOGE/Graphics/GetCompiledShaderPath.hpp"
#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"

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
    TriangleGraphicsPipeline::TriangleGraphicsPipeline(GraphicsCore& aCore, FinalGraphicsRenderPass& aRenderPass)
        : m_core{aCore}, m_renderPass{aRenderPass}
    {
        SOGE_INFO_LOG("Creating NVRHI triangle pipeline...");
        nvrhi::IDevice& nvrhiDevice = aCore.GetRawDevice();

        nvrhi::ShaderDesc vertexShaderDesc{};
        vertexShaderDesc.shaderType = nvrhi::ShaderType::Vertex;
        vertexShaderDesc.debugName = "SOGE triangle pipeline vertex shader";
        vertexShaderDesc.entryName = "VSMain";
        m_nvrhiVertexShader = LoadShader(aCore, vertexShaderDesc, "./resources/shaders/simple.hlsl", "VSMain");

        nvrhi::ShaderDesc pixelShaderDesc{};
        pixelShaderDesc.shaderType = nvrhi::ShaderType::Pixel;
        pixelShaderDesc.debugName = "SOGE triangle pipeline pixel shader";
        pixelShaderDesc.entryName = "PSMain";
        m_nvrhiPixelShader = LoadShader(aCore, pixelShaderDesc, "./resources/shaders/simple.hlsl", "PSMain");

        const std::array vertexAttributeDescArray{
            nvrhi::VertexAttributeDesc{
                .name = "position",
                .format = nvrhi::Format::RGB32_FLOAT,
                .offset = offsetof(Entity::Vertex, m_position),
                .elementStride = sizeof(Entity::Vertex),
            },
            nvrhi::VertexAttributeDesc{
                .name = "color",
                .format = nvrhi::Format::RGBA32_FLOAT,
                .offset = offsetof(Entity::Vertex, m_color),
                .elementStride = sizeof(Entity::Vertex),
            },
        };
        m_nvrhiInputLayout = nvrhiDevice.createInputLayout(vertexAttributeDescArray.data(),
                                                           static_cast<std::uint32_t>(vertexAttributeDescArray.size()),
                                                           m_nvrhiVertexShader);

        nvrhi::BindingLayoutDesc bindingLayoutDesc{};
        bindingLayoutDesc.visibility = nvrhi::ShaderType::All;
        bindingLayoutDesc.addItem(nvrhi::BindingLayoutItem::ConstantBuffer(0));
        m_nvrhiBindingLayout = nvrhiDevice.createBindingLayout(bindingLayoutDesc);

        nvrhi::GraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.inputLayout = m_nvrhiInputLayout;
        pipelineDesc.VS = m_nvrhiVertexShader;
        pipelineDesc.PS = m_nvrhiPixelShader;
        pipelineDesc.bindingLayouts = {m_nvrhiBindingLayout};
        // no need to create pipeline for each frame buffer, all of them are compatible with the first one
        nvrhi::IFramebuffer& compatibleFramebuffer = aRenderPass.GetFramebuffer();
        m_nvrhiGraphicsPipeline = nvrhiDevice.createGraphicsPipeline(pipelineDesc, &compatibleFramebuffer);
    }

    TriangleGraphicsPipeline::TriangleGraphicsPipeline(TriangleGraphicsPipeline&& aOther) noexcept
        : m_core{aOther.m_core}, m_renderPass{aOther.m_renderPass}
    {
        swap(aOther);
    }

    TriangleGraphicsPipeline& TriangleGraphicsPipeline::operator=(TriangleGraphicsPipeline&& aOther) noexcept
    {
        swap(aOther);
        return *this;
    }

    void TriangleGraphicsPipeline::swap(TriangleGraphicsPipeline& aOther) noexcept
    {
        using std::swap;

        eastl::swap(m_core, aOther.m_core);
        eastl::swap(m_renderPass, aOther.m_renderPass);

        swap(m_nvrhiVertexShader, aOther.m_nvrhiVertexShader);
        swap(m_nvrhiInputLayout, aOther.m_nvrhiInputLayout);
        swap(m_nvrhiPixelShader, aOther.m_nvrhiPixelShader);
        swap(m_nvrhiBindingLayout, aOther.m_nvrhiBindingLayout);
        swap(m_nvrhiGraphicsPipeline, aOther.m_nvrhiGraphicsPipeline);

        swap(m_commandLists, aOther.m_commandLists);
    }

    TriangleGraphicsPipeline::~TriangleGraphicsPipeline()
    {
        m_commandLists.clear();

        SOGE_INFO_LOG("Destroying NVRHI triangle pipeline...");
        m_nvrhiGraphicsPipeline = nullptr;
        m_nvrhiBindingLayout = nullptr;
        m_nvrhiPixelShader = nullptr;
        m_nvrhiInputLayout = nullptr;
        m_nvrhiVertexShader = nullptr;
    }

    nvrhi::IGraphicsPipeline& TriangleGraphicsPipeline::GetGraphicsPipeline() noexcept
    {
        return *m_nvrhiGraphicsPipeline;
    }

    auto TriangleGraphicsPipeline::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera,
                                           const Entities aEntities) -> CommandLists
    {
        m_commandLists.clear();

        nvrhi::CommandListParameters commandListDesc{};
        commandListDesc.enableImmediateExecution = false;

        nvrhi::IDevice& device = m_core.get().GetRawDevice();
        const nvrhi::CommandListHandle drawCommandList = device.createCommandList(commandListDesc);

        {
            GraphicsCommandListGuard commandList{*drawCommandList};

            FinalGraphicsRenderPass& renderPass = m_renderPass;
            renderPass.ClearFramebuffer(commandList);

            for (auto&& entityRef : aEntities)
            {
                const auto entity = dynamic_cast<Entity*>(&entityRef.get());
                if (entity == nullptr)
                {
                    continue;
                }

                const Entity::ConstantBuffer constantBuffer{
                    .m_modelViewProjection =
                        aCamera.GetProjectionMatrix() * aCamera.m_transform.ViewMatrix() * entity->GetWorldMatrix({}),
                };
                commandList->writeBuffer(entity->GetConstantBuffer({}), &constantBuffer, sizeof(constantBuffer));

                auto vertexBuffer = entity->GetVertexBuffer({});
                auto indexBuffer = entity->GetIndexBuffer({});

                nvrhi::GraphicsState graphicsState{};
                graphicsState.pipeline = &GetGraphicsPipeline();
                graphicsState.framebuffer = &renderPass.GetFramebuffer();
                graphicsState.bindings = {entity->GetBindingSet({})};
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
                commandList->setGraphicsState(graphicsState);

                nvrhi::DrawArguments drawArguments{};
                if (indexBuffer != nullptr)
                {
                    const auto& desc = indexBuffer->getDesc();
                    drawArguments.vertexCount = static_cast<std::uint32_t>(desc.byteSize / sizeof(Entity::Index));

                    commandList->drawIndexed(drawArguments);
                }
                else if (vertexBuffer != nullptr)
                {
                    const auto& desc = vertexBuffer->getDesc();
                    drawArguments.vertexCount = static_cast<std::uint32_t>(desc.byteSize / sizeof(Entity::Vertex));

                    commandList->draw(drawArguments);
                }
            }
        }

        m_commandLists.push_back(drawCommandList);
        return m_commandLists;
    }
}
