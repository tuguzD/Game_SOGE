#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsPipeline.hpp"

#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"
#include "SOGE/Graphics/GraphicsModule.hpp"
#include "SOGE/Graphics/GraphicsSwapchain.hpp"
#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"

#include <filesystem>
#include <fstream>


namespace
{
    using Vertex = soge::D3D12GraphicsPipeline::Vertex;

    constexpr std::array g_vertices{
        Vertex{
            .m_position = glm::vec4{-0.5f, 0.5f, 0.0f, 0.0f},
            .m_color = glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},
        },
        Vertex{
            .m_position = glm::vec4{0.5f, 0.5f, 0.0f, 0.0f},
            .m_color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},
        },
        Vertex{
            .m_position = glm::vec4{0.0f, -0.5f, 0.0f, 0.0f},
            .m_color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
        },
    };

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
    D3D12GraphicsPipeline::D3D12GraphicsPipeline(D3D12GraphicsCore& aCore) : m_core{aCore}
    {
        // TODO: move code below into pipeline class
        SOGE_INFO_LOG("Creating NVRHI simple pipeline...");
        nvrhi::IDevice& nvrhiDevice = m_core.get().GetRawDevice();

        nvrhi::ShaderDesc vertexShaderDesc{};
        vertexShaderDesc.shaderType = nvrhi::ShaderType::Vertex;
        vertexShaderDesc.debugName = "SOGE vertex shader";
        vertexShaderDesc.entryName = "VSMain";
        m_nvrhiVertexShader = LoadShader(aCore, vertexShaderDesc, "./resources/shaders/simple.hlsl", "VSMain");

        nvrhi::ShaderDesc pixelShaderDesc{};
        pixelShaderDesc.shaderType = nvrhi::ShaderType::Pixel;
        pixelShaderDesc.debugName = "SOGE pixel shader";
        pixelShaderDesc.entryName = "PSMain";
        m_nvrhiPixelShader = LoadShader(aCore, pixelShaderDesc, "./resources/shaders/simple.hlsl", "PSMain");

        const std::array vertexAttributeDescArray{
            nvrhi::VertexAttributeDesc{
                .name = "position",
                .format = nvrhi::Format::RGB32_FLOAT,
                .offset = offsetof(Vertex, m_position),
                .elementStride = sizeof(Vertex),
            },
            nvrhi::VertexAttributeDesc{
                .name = "color",
                .format = nvrhi::Format::RGBA32_FLOAT,
                .offset = offsetof(Vertex, m_color),
                .elementStride = sizeof(Vertex),
            },
        };
        m_nvrhiInputLayout = nvrhiDevice.createInputLayout(vertexAttributeDescArray.data(),
                                                           static_cast<std::uint32_t>(vertexAttributeDescArray.size()),
                                                           m_nvrhiVertexShader);

        nvrhi::BindingLayoutDesc bindingLayoutDesc{};
        bindingLayoutDesc.visibility = nvrhi::ShaderType::All;
        m_nvrhiBindingLayout = nvrhiDevice.createBindingLayout(bindingLayoutDesc);

        nvrhi::GraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.inputLayout = m_nvrhiInputLayout;
        pipelineDesc.VS = m_nvrhiVertexShader;
        pipelineDesc.PS = m_nvrhiPixelShader;
        pipelineDesc.bindingLayouts = {m_nvrhiBindingLayout};
        // no need to create pipeline for each frame buffer, all of them are compatible with the first one
        m_nvrhiGraphicsPipeline = nvrhiDevice.createGraphicsPipeline(pipelineDesc, aCore.m_nvrhiFramebuffers[0]);

        // TODO: move code below into component class
        SOGE_INFO_LOG("Creating NVRHI vertex buffer...");
        nvrhi::BufferDesc vertexBufferDesc{};
        vertexBufferDesc.byteSize = sizeof(g_vertices);
        vertexBufferDesc.isVertexBuffer = true;
        vertexBufferDesc.initialState = nvrhi::ResourceStates::VertexBuffer;
        vertexBufferDesc.keepInitialState = true;
        vertexBufferDesc.debugName = "SOGE vertex buffer";
        m_nvrhiVertexBuffer = nvrhiDevice.createBuffer(vertexBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        m_nvrhiBindingSet = nvrhiDevice.createBindingSet(bindingSetDesc, m_nvrhiBindingLayout);

        const nvrhi::CommandListHandle verticesCommandList = nvrhiDevice.createCommandList();
        {
            GraphicsCommandListGuard commandList{*verticesCommandList};
            commandList->writeBuffer(m_nvrhiVertexBuffer, g_vertices.data(), sizeof(g_vertices));
        }
        nvrhiDevice.executeCommandList(verticesCommandList, nvrhi::CommandQueue::Graphics);
    }

    D3D12GraphicsPipeline::D3D12GraphicsPipeline(D3D12GraphicsPipeline&& aOther) noexcept : m_core{aOther.m_core}
    {
        swap(aOther);
    }

    D3D12GraphicsPipeline& D3D12GraphicsPipeline::operator=(D3D12GraphicsPipeline&& aOther) noexcept
    {
        swap(aOther);
        return *this;
    }

    void D3D12GraphicsPipeline::swap(D3D12GraphicsPipeline& aOther) noexcept
    {
        using std::swap;

        eastl::swap(m_core, aOther.m_core);

        swap(m_commandLists, aOther.m_commandLists);
        swap(m_commandListRefs, aOther.m_commandListRefs);

        swap(m_nvrhiVertexShader, aOther.m_nvrhiVertexShader);
        swap(m_nvrhiInputLayout, aOther.m_nvrhiInputLayout);
        swap(m_nvrhiPixelShader, aOther.m_nvrhiPixelShader);
        swap(m_nvrhiBindingLayout, aOther.m_nvrhiBindingLayout);
        swap(m_nvrhiGraphicsPipeline, aOther.m_nvrhiGraphicsPipeline);

        swap(m_nvrhiVertexBuffer, aOther.m_nvrhiVertexBuffer);
        swap(m_nvrhiBindingSet, aOther.m_nvrhiBindingSet);
    }

    D3D12GraphicsPipeline::~D3D12GraphicsPipeline()
    {
        m_commandLists.clear();
        m_commandListRefs.clear();

        // TODO: move code below into component class
        if (m_nvrhiBindingSet != nullptr)
        {
            SOGE_INFO_LOG("Destroying NVRHI binding set...");
            m_nvrhiBindingSet = nullptr;
        }
        if (m_nvrhiVertexBuffer != nullptr)
        {
            SOGE_INFO_LOG("Destroying NVRHI vertex buffer...");
            m_nvrhiVertexBuffer = nullptr;
        }

        if (m_nvrhiGraphicsPipeline != nullptr)
        {
            SOGE_INFO_LOG("Destroying NVRHI simple pipeline...");
            m_nvrhiGraphicsPipeline = nullptr;
            m_nvrhiBindingLayout = nullptr;
            m_nvrhiPixelShader = nullptr;
            m_nvrhiInputLayout = nullptr;
            m_nvrhiVertexShader = nullptr;
        }
    }

    auto D3D12GraphicsPipeline::Update(float aDeltaTime) -> CommandLists
    {
        m_commandLists.clear();
        m_commandListRefs.clear();

        nvrhi::CommandListParameters commandListDesc{};
        commandListDesc.enableImmediateExecution = false;

        nvrhi::IDevice& nvrhiDevice = m_core.get().GetRawDevice();
        const nvrhi::CommandListHandle triangleCommandList = nvrhiDevice.createCommandList(commandListDesc);
        m_commandLists.emplace_back(triangleCommandList);
        {
            GraphicsCommandListGuard commandList{*triangleCommandList};

            const auto currentFrameIndex = m_core.get().m_swapChain->GetCurrentTextureIndex();
            nvrhi::IFramebuffer& currentFramebuffer = *m_core.get().m_nvrhiFramebuffers[currentFrameIndex];
            const nvrhi::FramebufferInfoEx& framebufferInfo = currentFramebuffer.getFramebufferInfo();

            nvrhi::GraphicsState graphicsState{};
            graphicsState.pipeline = m_nvrhiGraphicsPipeline;
            graphicsState.framebuffer = &currentFramebuffer;
            graphicsState.bindings = {m_nvrhiBindingSet};
            graphicsState.vertexBuffers = {
                nvrhi::VertexBufferBinding{.buffer = m_nvrhiVertexBuffer, .slot = 0, .offset = 0},
            };
            graphicsState.viewport.addViewportAndScissorRect(framebufferInfo.getViewport());
            commandList->setGraphicsState(graphicsState);

            nvrhi::DrawArguments drawArguments{};
            drawArguments.vertexCount = static_cast<std::uint32_t>(g_vertices.size());
            commandList->draw(drawArguments);
        }

        m_commandListRefs.reserve(m_commandLists.size());
        for (const auto& commandList : m_commandLists)
        {
            m_commandListRefs.emplace_back(*commandList);
        }
        return m_commandListRefs;
    }
}
