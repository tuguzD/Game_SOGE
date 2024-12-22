#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"

#include "SOGE/Graphics/Exceptions/NRIException.hpp"
#include "SOGE/Graphics/Generic/Vertex.hpp"
#include "SOGE/Window/Window.hpp"

#include <Extensions/NRIWrapperD3D12.h>

#ifdef SOGE_DEBUG
#include <nvrhi/validation.h>
#endif

#include <fstream>


namespace
{
    // Taken from NRI source code (function `GetResultFromHRESULT`)
    nri::Result ConvertResult(const HRESULT aResult)
    {
        if (SUCCEEDED(aResult))
            return nri::Result::SUCCESS;

        if (aResult == E_INVALIDARG || aResult == E_POINTER || aResult == E_HANDLE)
            return nri::Result::INVALID_ARGUMENT;

        if (aResult == DXGI_ERROR_UNSUPPORTED)
            return nri::Result::UNSUPPORTED;

        if (aResult == DXGI_ERROR_DEVICE_REMOVED || aResult == DXGI_ERROR_DEVICE_RESET)
            return nri::Result::DEVICE_LOST;

        if (aResult == E_OUTOFMEMORY)
            return nri::Result::OUT_OF_MEMORY;

        return nri::Result::FAILURE;
    }

    constexpr std::array g_vertices{
        soge::Vertex{
            .m_position = glm::vec4{-0.5f, 0.5f, 0.0f, 0.0f},
            .m_color = glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},
        },
        soge::Vertex{
            .m_position = glm::vec4{0.5f, 0.5f, 0.0f, 0.0f},
            .m_color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},
        },
        soge::Vertex{
            .m_position = glm::vec4{0.0f, -0.5f, 0.0f, 0.0f},
            .m_color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
        },
    };
}

namespace soge
{
    D3D12GraphicsCore::D3D12GraphicsCore()
        : m_nriCallbackInterface(), m_nriAllocationCallbacks(), m_nriSwapChain(nullptr), m_totalFrameCount()
    {
        SOGE_INFO_LOG("Creating D3D12 render backend...");
        m_nriCallbackInterface.MessageCallback = NriMessageCallback;

        SOGE_INFO_LOG("Choosing best render device...");
        nri::AdapterDesc bestAdapterDesc{};
        std::uint32_t adapterDescNum = 1;
        NRIThrowIfFailed(nri::nriEnumerateAdapters(&bestAdapterDesc, adapterDescNum), "choosing best render device");
        SOGE_INFO_LOG(R"(Rendering device "{}" was chosen...)", bestAdapterDesc.name);

        SOGE_INFO_LOG("Creating NRI render device...");
        nri::DeviceCreationDesc deviceCreationDesc{};
        deviceCreationDesc.graphicsAPI = nri::GraphicsAPI::D3D12;
#ifdef SOGE_DEBUG
        deviceCreationDesc.enableGraphicsAPIValidation = true;
        deviceCreationDesc.enableNRIValidation = true;
#endif
        deviceCreationDesc.adapterDesc = &bestAdapterDesc;
        deviceCreationDesc.callbackInterface = m_nriCallbackInterface;
        deviceCreationDesc.allocationCallbacks = m_nriAllocationCallbacks;
        NRIThrowIfFailed(nri::nriCreateDevice(deviceCreationDesc, m_nriInitDevice), "creating render device");

        SOGE_INFO_LOG("Retrieving NRI interfaces...");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriInitDevice, NRI_INTERFACE(nri::CoreInterface),
                                              static_cast<nri::CoreInterface*>(&m_nriInterface)),
                         "retrieving NRI core interface");
        m_nriInterface.SetDeviceDebugName(*m_nriInitDevice, "SOGE device");

        NRIThrowIfFailed(nri::nriGetInterface(*m_nriInitDevice, NRI_INTERFACE(nri::HelperInterface),
                                              static_cast<nri::HelperInterface*>(&m_nriInterface)),
                         "retrieving NRI helper interface");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriInitDevice, NRI_INTERFACE(nri::StreamerInterface),
                                              static_cast<nri::StreamerInterface*>(&m_nriInterface)),
                         "retrieving NRI streamer interface");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriInitDevice, NRI_INTERFACE(nri::SwapChainInterface),
                                              static_cast<nri::SwapChainInterface*>(&m_nriInterface)),
                         "retrieving NRI swap chain interface");

        SOGE_INFO_LOG("Creating graphics command queue...");
        D3D12_COMMAND_QUEUE_DESC graphicsCommandQueueDesc{};
        graphicsCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        graphicsCommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        graphicsCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        nvrhi::RefCountPtr<ID3D12CommandQueue> graphicsCommandQueue;
        const auto d3d12Device = static_cast<ID3D12Device*>(m_nriInterface.GetDeviceNativeObject(*m_nriInitDevice));
        NRIThrowIfFailed(ConvertResult(d3d12Device->CreateCommandQueue(&graphicsCommandQueueDesc,
                                                                       IID_PPV_ARGS(&graphicsCommandQueue))),
                         "creating graphics command queue");

        SOGE_INFO_LOG("Creating NRI device wrapper from D3D12 device...");
        nri::DeviceCreationD3D12Desc deviceCreationD3D12Desc{};
        deviceCreationD3D12Desc.d3d12Device = d3d12Device;
        deviceCreationD3D12Desc.d3d12GraphicsQueue = graphicsCommandQueue.Get();
        deviceCreationD3D12Desc.callbackInterface = m_nriCallbackInterface;
        deviceCreationD3D12Desc.allocationCallbacks = m_nriAllocationCallbacks;
        NRIThrowIfFailed(nri::nriCreateDeviceFromD3D12Device(deviceCreationD3D12Desc, m_nriDevice),
                         "creating NRI device wrapper from D3D12 device");

        SOGE_INFO_LOG("Retrieving NRI interfaces from device wrapper...");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::CoreInterface),
                                              static_cast<nri::CoreInterface*>(&m_nriInterface)),
                         "retrieving NRI core interface from device wrapper");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::HelperInterface),
                                              static_cast<nri::HelperInterface*>(&m_nriInterface)),
                         "retrieving NRI helper interface from device wrapper");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::StreamerInterface),
                                              static_cast<nri::StreamerInterface*>(&m_nriInterface)),
                         "retrieving NRI streamer interface from device wrapper");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::SwapChainInterface),
                                              static_cast<nri::SwapChainInterface*>(&m_nriInterface)),
                         "retrieving NRI swap chain interface from device wrapper");

        NRIThrowIfFailed(
            m_nriInterface.GetCommandQueue(*m_nriDevice, nri::CommandQueueType::GRAPHICS, m_nriGraphicsCommandQueue),
            "retrieving NRI graphics command queue");
        m_nriInterface.SetCommandQueueDebugName(*m_nriGraphicsCommandQueue, "SOGE graphics command queue");

        SOGE_INFO_LOG("Creating NVRHI render device...");
        const nvrhi::d3d12::DeviceDesc deviceDesc{
            .errorCB = &m_nvrhiMessageCallback,
            .pDevice = d3d12Device,
            .pGraphicsCommandQueue = graphicsCommandQueue.Detach(),
        };
        m_nvrhiDevice = nvrhi::d3d12::createDevice(deviceDesc);
#ifdef SOGE_DEBUG
        m_nvrhiDevice = nvrhi::validation::createValidationLayer(m_nvrhiDevice);
#endif
    }

    D3D12GraphicsCore::~D3D12GraphicsCore()
    {
        SOGE_INFO_LOG("Destroying D3D12 render backend...");

        if (m_nriGraphicsCommandQueue != nullptr)
        {
            m_nriInterface.WaitForIdle(*m_nriGraphicsCommandQueue);
        }
        m_nvrhiDevice->waitForIdle();
        m_nvrhiDevice->runGarbageCollection();

        DestroySwapChain();
        DestroyDevice();
    }

    void D3D12GraphicsCore::DestroySwapChain()
    {
        if (m_nriSwapChain == nullptr)
        {
            return;
        }

        // TODO: move code below into component class
        SOGE_INFO_LOG("Destroying vertex buffer...");
        m_nvrhiBindingSet = nullptr;
        m_nvrhiVertexBuffer = nullptr;

        // TODO: move code below into pipeline class
        SOGE_INFO_LOG("Destroying simple pipeline...");
        m_nvrhiGraphicsPipeline = nullptr;
        m_nvrhiBindingLayout = nullptr;
        m_nvrhiPixelShader = nullptr;
        m_nvrhiInputLayout = nullptr;
        m_nvrhiVertexShader = nullptr;
        // end of TODO

        SOGE_INFO_LOG("Destroying NVRHI back buffers...");
        m_backBuffers.clear();

        SOGE_INFO_LOG("Destroying NRI swap chain...");
        m_nriInterface.DestroySwapChain(*m_nriSwapChain);
        m_nriSwapChain = nullptr;
    }

    void D3D12GraphicsCore::DestroyDevice()
    {
        SOGE_INFO_LOG("Destroying NVRHI render device...");
        m_nvrhiDevice = nullptr;

        if (m_nriDevice == nullptr)
        {
            return;
        }
        SOGE_INFO_LOG("Destroying NRI render device wrapper...");
        nri::nriDestroyDevice(*m_nriDevice);
        m_nriDevice = nullptr;
        m_nriGraphicsCommandQueue = nullptr;

        if (m_nriInitDevice == nullptr)
        {
            return;
        }
        SOGE_INFO_LOG("Destroying NRI render device...");
        nri::nriDestroyDevice(*m_nriInitDevice);
        m_nriInitDevice = nullptr;
    }

    void D3D12GraphicsCore::SetRenderTarget(const Window& aWindow)
    {
        DestroySwapChain();

        SOGE_INFO_LOG("Creating NRI swap chain for window...");
        nri::SwapChainDesc swapChainDesc{};

        nri::Window nriWindow{};
        nriWindow.windows = nri::WindowsWindow{static_cast<HWND>(aWindow.GetNativeHandler())};
        swapChainDesc.window = nriWindow;

        swapChainDesc.commandQueue = m_nriGraphicsCommandQueue;
        swapChainDesc.width = static_cast<nri::Dim_t>(aWindow.GetWidth());
        swapChainDesc.height = static_cast<nri::Dim_t>(aWindow.GetHeight());
        swapChainDesc.textureNum = 2;
        swapChainDesc.format = nri::SwapChainFormat::BT709_G10_16BIT;
        swapChainDesc.waitable = true; // TODO: set queuedFrameNum to 2 (in the future, not now)

        NRIThrowIfFailed(m_nriInterface.CreateSwapChain(*m_nriDevice, swapChainDesc, m_nriSwapChain),
                         "creating a swap chain for window");
        m_nriInterface.SetSwapChainDebugName(*m_nriSwapChain, "SOGE swap chain");

        std::uint32_t swapChainColorTextureCount;
        nri::Texture* const* swapChainColorTextures =
            m_nriInterface.GetSwapChainTextures(*m_nriSwapChain, swapChainColorTextureCount);
        SOGE_INFO_LOG("Swap chain texture count: {}", swapChainColorTextureCount);

        m_backBuffers.reserve(swapChainColorTextureCount);
        for (std::size_t index = 0; index < swapChainColorTextureCount; index++)
        {
            SOGE_INFO_LOG("Creating NVRHI swap chain color texture (frame {})...", index);

            nri::Texture* const nriColorTexture = swapChainColorTextures[index];
            m_nriInterface.SetTextureDebugName(*nriColorTexture,
                                               fmt::format("SOGE swap chain color texture (frame {})", index).c_str());

            const nri::TextureDesc& nriColorTextureDesc = m_nriInterface.GetTextureDesc(*nriColorTexture);
            assert(nriColorTextureDesc.type == nri::TextureType::TEXTURE_2D);
            assert(nriColorTextureDesc.usage & nri::TextureUsageBits::COLOR_ATTACHMENT &&
                   nriColorTextureDesc.usage & nri::TextureUsageBits::SHADER_RESOURCE);
            assert(nriColorTextureDesc.format == nri::Format::RGBA16_SFLOAT);

            nvrhi::TextureDesc nvrhiColorTextureDesc{};
            nvrhiColorTextureDesc.dimension = nvrhi::TextureDimension::Texture2D;
            nvrhiColorTextureDesc.format = nvrhi::Format::RGBA16_FLOAT;
            nvrhiColorTextureDesc.width = nriColorTextureDesc.width;
            nvrhiColorTextureDesc.height = nriColorTextureDesc.height;
            nvrhiColorTextureDesc.isRenderTarget = true;
            nvrhiColorTextureDesc.isShaderResource = true;
            nvrhiColorTextureDesc.depth = nriColorTextureDesc.depth;
            nvrhiColorTextureDesc.mipLevels = nriColorTextureDesc.mipNum;
            nvrhiColorTextureDesc.arraySize = nriColorTextureDesc.layerNum;
            nvrhiColorTextureDesc.sampleCount = nriColorTextureDesc.sampleNum;
            nvrhiColorTextureDesc.initialState = nvrhi::ResourceStates::Present;
            nvrhiColorTextureDesc.keepInitialState = true;

            nvrhi::TextureHandle nvrhiColorTexture = m_nvrhiDevice->createHandleForNativeTexture(
                nvrhi::ObjectTypes::D3D12_Resource, m_nriInterface.GetTextureNativeObject(*nriColorTexture),
                nvrhiColorTextureDesc);

            SOGE_INFO_LOG("Creating NVRHI framebuffer (frame {})...", index);
            nvrhi::FramebufferDesc framebufferDesc{};
            framebufferDesc.addColorAttachment(nvrhiColorTexture);

            nvrhi::FramebufferHandle nvrhiFramebuffer = m_nvrhiDevice->createFramebuffer(framebufferDesc);

            BackBuffer backBuffer{
                .m_nvrhiColorAttachments = {nvrhiColorTexture},
                .m_nvrhiFramebuffer = nvrhiFramebuffer,
            };
            m_backBuffers.push_back(backBuffer);
        }

        // TODO: move code below into pipeline class
        SOGE_INFO_LOG("Creating NVRHI simple pipeline...");

        // TODO: move file in a separate folder
        std::ifstream vertexShaderFile("simple.vs.bin", std::ios::in | std::ios::binary);
        std::vector<std::uint8_t> vertexShaderBinary(std::istreambuf_iterator(vertexShaderFile), {});
        vertexShaderFile.close();

        nvrhi::ShaderDesc vertexShaderDesc{};
        vertexShaderDesc.shaderType = nvrhi::ShaderType::Vertex;
        vertexShaderDesc.debugName = "SOGE vertex shader";
        vertexShaderDesc.entryName = "VSMain";
        m_nvrhiVertexShader =
            m_nvrhiDevice->createShader(vertexShaderDesc, vertexShaderBinary.data(), vertexShaderBinary.size());

        // TODO: move file in a separate folder
        std::ifstream pixelShaderFile("simple.ps.bin", std::ios::in | std::ios::binary);
        std::vector<std::uint8_t> pixelShaderBinary(std::istreambuf_iterator(pixelShaderFile), {});
        pixelShaderFile.close();

        nvrhi::ShaderDesc pixelShaderDesc{};
        pixelShaderDesc.shaderType = nvrhi::ShaderType::Pixel;
        pixelShaderDesc.debugName = "SOGE pixel shader";
        pixelShaderDesc.entryName = "PSMain";
        m_nvrhiPixelShader =
            m_nvrhiDevice->createShader(pixelShaderDesc, pixelShaderBinary.data(), pixelShaderBinary.size());

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
        m_nvrhiInputLayout = m_nvrhiDevice->createInputLayout(
            vertexAttributeDescArray.data(), static_cast<std::uint32_t>(vertexAttributeDescArray.size()),
            m_nvrhiVertexShader);

        nvrhi::BindingLayoutDesc bindingLayoutDesc{};
        bindingLayoutDesc.visibility = nvrhi::ShaderType::All;
        m_nvrhiBindingLayout = m_nvrhiDevice->createBindingLayout(bindingLayoutDesc);

        nvrhi::GraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.inputLayout = m_nvrhiInputLayout;
        pipelineDesc.VS = m_nvrhiVertexShader;
        pipelineDesc.PS = m_nvrhiPixelShader;
        pipelineDesc.bindingLayouts = {m_nvrhiBindingLayout};
        pipelineDesc.renderState.depthStencilState.depthTestEnable = false;
        // no need to create pipeline for each frame buffer, all of them are compatible with the first one
        nvrhi::FramebufferHandle framebuffer = m_backBuffers[0].m_nvrhiFramebuffer;
        m_nvrhiGraphicsPipeline = m_nvrhiDevice->createGraphicsPipeline(pipelineDesc, framebuffer);

        // TODO: move code below into component class
        SOGE_INFO_LOG("Creating vertex buffer...");
        nvrhi::BufferDesc vertexBufferDesc{};
        vertexBufferDesc.byteSize = sizeof(g_vertices);
        vertexBufferDesc.isVertexBuffer = true;
        vertexBufferDesc.initialState = nvrhi::ResourceStates::VertexBuffer;
        vertexBufferDesc.keepInitialState = true;
        vertexBufferDesc.debugName = "SOGE vertex buffer";
        m_nvrhiVertexBuffer = m_nvrhiDevice->createBuffer(vertexBufferDesc);

        SOGE_INFO_LOG("Creating binding set...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        m_nvrhiBindingSet = m_nvrhiDevice->createBindingSet(bindingSetDesc, m_nvrhiBindingLayout);

        const nvrhi::CommandListHandle commandList = m_nvrhiDevice->createCommandList();
        commandList->open();
        commandList->writeBuffer(m_nvrhiVertexBuffer, g_vertices.data(), sizeof(g_vertices));
        commandList->close();
        m_nvrhiDevice->executeCommandList(commandList, nvrhi::CommandQueue::Graphics);
    }

    void D3D12GraphicsCore::Update(float aDeltaTime)
    {
        SOGE_INFO_LOG("Rendering {} frame with input delta time of {}...", m_totalFrameCount, aDeltaTime);

        NRIThrowIfFailed(m_nriInterface.WaitForPresent(*m_nriSwapChain), "waiting for swap chain to present");
        m_nvrhiDevice->runGarbageCollection();

        const auto currentFrameIndex = m_nriInterface.AcquireNextSwapChainTexture(*m_nriSwapChain);
        const auto& [currentColorAttachments, currentFramebuffer] = m_backBuffers[currentFrameIndex];
        const nvrhi::FramebufferInfoEx& framebufferInfo = currentFramebuffer->getFramebufferInfo();
        const nvrhi::FramebufferDesc& framebufferDesc = currentFramebuffer->getDesc();

        const nvrhi::CommandListHandle commandList = m_nvrhiDevice->createCommandList();
        commandList->open();

        for (const auto& colorAttachment : framebufferDesc.colorAttachments)
        {
            commandList->clearTextureFloat(colorAttachment.texture, colorAttachment.subresources, nvrhi::Color{});
        }

        nvrhi::GraphicsState graphicsState{};
        graphicsState.pipeline = m_nvrhiGraphicsPipeline;
        graphicsState.framebuffer = currentFramebuffer;
        graphicsState.bindings = {m_nvrhiBindingSet};
        graphicsState.vertexBuffers = {
            nvrhi::VertexBufferBinding{.buffer = m_nvrhiVertexBuffer, .slot = 0, .offset = 0},
        };
        graphicsState.viewport = nvrhi::ViewportState().addViewportAndScissorRect(framebufferInfo.getViewport());
        commandList->setGraphicsState(graphicsState);

        nvrhi::DrawArguments drawArguments{};
        drawArguments.vertexCount = static_cast<std::uint32_t>(g_vertices.size());
        commandList->draw(drawArguments);

        commandList->close();
        m_nvrhiDevice->executeCommandList(commandList, nvrhi::CommandQueue::Graphics);

        NRIThrowIfFailed(m_nriInterface.QueuePresent(*m_nriSwapChain), "presenting swap chain");
        m_totalFrameCount++;
    }

    D3D12GraphicsCore::NvrhiMessageCallback::NvrhiMessageCallback(NvrhiMessageCallback&&) noexcept
    {
    }

    auto D3D12GraphicsCore::NvrhiMessageCallback::operator=(NvrhiMessageCallback&&) noexcept -> NvrhiMessageCallback&
    {
        return *this;
    }

    void D3D12GraphicsCore::NvrhiMessageCallback::message(const nvrhi::MessageSeverity aSeverity,
                                                          const char* aMessageText)
    {
        Logger::Level level{Logger::Level::trace};
        switch (aSeverity)
        {
        case nvrhi::MessageSeverity::Info:
            level = Logger::Level::info;
            break;
        case nvrhi::MessageSeverity::Warning:
            level = Logger::Level::warn;
            break;
        case nvrhi::MessageSeverity::Error:
            level = Logger::Level::err;
            break;
        case nvrhi::MessageSeverity::Fatal:
            level = Logger::Level::critical;
            break;
        }
        Logger::EngineLog(level, "[NVRHI] {}", aMessageText);
    }

    void D3D12GraphicsCore::NriMessageCallback(const nri::Message aMessageType, const char* aFile, std::uint32_t aLine,
                                               const char* aMessage, [[maybe_unused]] void* aUserArg)
    {
        Logger::Level level{Logger::Level::trace};
        switch (aMessageType)
        {
        case nri::Message::INFO:
            level = Logger::Level::info;
            break;
        case nri::Message::WARNING:
            level = Logger::Level::warn;
            break;
        case nri::Message::ERROR:
            level = Logger::Level::err;
            break;
        case nri::Message::MAX_NUM:
            break;
        }
        Logger::EngineLog(level, "[NRI] ({}:{}) {}", aFile, aLine, aMessage);
    }
}
