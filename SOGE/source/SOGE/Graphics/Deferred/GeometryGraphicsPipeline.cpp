#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/GeometryGraphicsPipeline.hpp"

#include "SOGE/Graphics/Utils/GetCompiledShaderPath.hpp"

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
    GeometryGraphicsPipeline::GeometryGraphicsPipeline(GraphicsCore& aCore, GeometryGraphicsRenderPass& aRenderPass)
        : m_core{aCore}, m_renderPass{aRenderPass}
    {
        SOGE_INFO_LOG("Creating NVRHI geometry pipeline...");
        nvrhi::IDevice& nvrhiDevice = aCore.GetRawDevice();

        nvrhi::ShaderDesc vertexShaderDesc{};
        vertexShaderDesc.shaderType = nvrhi::ShaderType::Vertex;
        vertexShaderDesc.debugName = "SOGE geometry pipeline vertex shader";
        vertexShaderDesc.entryName = "VSMain";
        m_nvrhiVertexShader =
            LoadShader(aCore, vertexShaderDesc, "./resources/shaders/deferred_geometry.hlsl", "VSMain");

        nvrhi::ShaderDesc pixelShaderDesc{};
        pixelShaderDesc.shaderType = nvrhi::ShaderType::Pixel;
        pixelShaderDesc.debugName = "SOGE geometry pipeline pixel shader";
        pixelShaderDesc.entryName = "PSMain";
        m_nvrhiPixelShader = LoadShader(aCore, pixelShaderDesc, "./resources/shaders/deferred_geometry.hlsl", "PSMain");

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
        nvrhi::IFramebuffer& compatibleFramebuffer = aRenderPass.GetFramebuffer();
        m_nvrhiGraphicsPipeline = nvrhiDevice.createGraphicsPipeline(pipelineDesc, &compatibleFramebuffer);
    }

    GeometryGraphicsPipeline::GeometryGraphicsPipeline(GeometryGraphicsPipeline&& aOther) noexcept
        : m_core{aOther.m_core}, m_renderPass{aOther.m_renderPass}
    {
        swap(aOther);
    }

    GeometryGraphicsPipeline& GeometryGraphicsPipeline::operator=(GeometryGraphicsPipeline&& aOther) noexcept
    {
        swap(aOther);
        return *this;
    }

    GeometryGraphicsPipeline::~GeometryGraphicsPipeline()
    {
        SOGE_INFO_LOG("Destroying NVRHI geometry pipeline...");
        m_nvrhiGraphicsPipeline = nullptr;
        m_nvrhiBindingLayout = nullptr;
        m_nvrhiPixelShader = nullptr;
        m_nvrhiInputLayout = nullptr;
        m_nvrhiVertexShader = nullptr;
    }

    void GeometryGraphicsPipeline::swap(GeometryGraphicsPipeline& aOther) noexcept
    {
        using std::swap;

        eastl::swap(m_core, aOther.m_core);
        eastl::swap(m_renderPass, aOther.m_renderPass);

        swap(m_nvrhiVertexShader, aOther.m_nvrhiVertexShader);
        swap(m_nvrhiInputLayout, aOther.m_nvrhiInputLayout);
        swap(m_nvrhiPixelShader, aOther.m_nvrhiPixelShader);
        swap(m_nvrhiBindingLayout, aOther.m_nvrhiBindingLayout);
        swap(m_nvrhiGraphicsPipeline, aOther.m_nvrhiGraphicsPipeline);
    }

    nvrhi::IGraphicsPipeline& GeometryGraphicsPipeline::GetGraphicsPipeline() noexcept
    {
        return *m_nvrhiGraphicsPipeline;
    }

    void GeometryGraphicsPipeline::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                                           nvrhi::ICommandList& aCommandList)
    {
        const Entity::ConstantBuffer constantBuffer{
            .m_model = aEntity.GetWorldMatrix({}),
            .m_view = aCamera.m_transform.ViewMatrix(),
            .m_projection = aCamera.GetProjectionMatrix(),
        };
        aCommandList.writeBuffer(aEntity.GetConstantBuffer({}), &constantBuffer, sizeof(constantBuffer));

        const auto vertexBuffer = aEntity.GetVertexBuffer({});
        const auto indexBuffer = aEntity.GetIndexBuffer({});

        nvrhi::GraphicsState graphicsState{};
        graphicsState.pipeline = &GetGraphicsPipeline();
        graphicsState.framebuffer = &m_renderPass.get().GetFramebuffer();
        graphicsState.bindings = {aEntity.GetBindingSet({})};
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
