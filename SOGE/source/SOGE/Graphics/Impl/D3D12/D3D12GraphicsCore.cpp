#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"

#include "SOGE/Graphics/Exceptions/NRIException.hpp"
#include "SOGE/Graphics/Generic/Vertex.hpp"
#include "SOGE/Window/Window.hpp"

#include <Extensions/NRIWrapperD3D12.h>

#include <nvrhi/validation.h>


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
}

namespace soge
{
    D3D12GraphicsCore::D3D12GraphicsCore()
        : m_nriCallbackInterface(), m_nriAllocationCallbacks(), m_nriSwapChain(nullptr)
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
        deviceCreationDesc.enableGraphicsAPIValidation = true;
        deviceCreationDesc.enableNRIValidation = true;
        deviceCreationDesc.adapterDesc = &bestAdapterDesc;
        deviceCreationDesc.callbackInterface = m_nriCallbackInterface;
        deviceCreationDesc.allocationCallbacks = m_nriAllocationCallbacks;
        NRIThrowIfFailed(nri::nriCreateDevice(deviceCreationDesc, m_nriInitDevice), "creating render device");

        SOGE_INFO_LOG("Retrieving NRI interfaces...");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriInitDevice, NRI_INTERFACE(nri::CoreInterface),
                                              static_cast<nri::CoreInterface*>(&m_nriInterface)),
                         "retrieving NRI core interface");
        m_nriInterface.SetDeviceDebugName(*m_nriInitDevice, "SOGE D3D12 GPU device");

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
        NRIThrowIfFailed(ConvertResult(graphicsCommandQueue->SetName(L"SOGE graphics command queue")),
                         "setting debug name for graphics command queue");

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

        SOGE_INFO_LOG("Creating NVRHI render device...");
        const nvrhi::d3d12::DeviceDesc deviceDesc{
            .errorCB = &m_nvrhiMessageCallback,
            .pDevice = d3d12Device,
            .pGraphicsCommandQueue = graphicsCommandQueue.Detach(),
        };
        m_nvrhiDevice = nvrhi::d3d12::createDevice(deviceDesc);
        m_nvrhiDevice = nvrhi::validation::createValidationLayer(m_nvrhiDevice);
    }

    D3D12GraphicsCore::~D3D12GraphicsCore()
    {
        SOGE_INFO_LOG("Destroying D3D12 render backend...");

        DestroySwapChain();
        DestroyDevice();
    }

    void D3D12GraphicsCore::DestroySwapChain()
    {
        if (m_nriSwapChain == nullptr)
        {
            return;
        }

        // TODO: move code below into pipeline class
        SOGE_INFO_LOG("Destroying simple pipeline...");
        m_nvrhiGraphicsPipeline = nullptr;
        m_nvrhiBindingLayout = nullptr;
        m_nvrhiPixelShader = nullptr;
        m_nvrhiInputLayout = nullptr;
        m_nvrhiVertexShader = nullptr;
        // end of TODO

        SOGE_INFO_LOG("Destroying NVRHI framebuffer...");
        m_nvrhiFramebuffer = nullptr;

        SOGE_INFO_LOG("Destroying NVRHI swap chain textures...");
        m_nvrhiSwapChainTextures.clear();

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

        nri::CommandQueue* commandQueue;
        NRIThrowIfFailed(m_nriInterface.GetCommandQueue(*m_nriDevice, nri::CommandQueueType::GRAPHICS, commandQueue),
                         "retrieving graphics command queue");
        swapChainDesc.commandQueue = commandQueue;

        swapChainDesc.width = static_cast<nri::Dim_t>(aWindow.GetWidth());
        swapChainDesc.height = static_cast<nri::Dim_t>(aWindow.GetHeight());
        swapChainDesc.textureNum = 2;
        swapChainDesc.format = nri::SwapChainFormat::BT709_G10_16BIT;

        NRIThrowIfFailed(m_nriInterface.CreateSwapChain(*m_nriDevice, swapChainDesc, m_nriSwapChain),
                         "creating a graphics swap chain for window");
        m_nriInterface.SetSwapChainDebugName(*m_nriSwapChain, "SOGE graphics swap chain");

        std::uint32_t swapChainTextureCount;
        const auto swapChainTextures = m_nriInterface.GetSwapChainTextures(*m_nriSwapChain, swapChainTextureCount);
        SOGE_INFO_LOG("Swap chain texture count: {}", swapChainTextureCount);

        m_nvrhiSwapChainTextures.reserve(swapChainTextureCount);
        for (std::size_t i = 0; i < swapChainTextureCount; i++)
        {
            SOGE_INFO_LOG("Creating NVRHI texture {} from NRI texture...", i);

            const auto nriTexture = swapChainTextures[i];
            const auto& nriTextureDesc = m_nriInterface.GetTextureDesc(*nriTexture);
            assert(nriTextureDesc.type == nri::TextureType::TEXTURE_2D);
            assert(nriTextureDesc.usage & nri::TextureUsageBits::COLOR_ATTACHMENT &&
                   nriTextureDesc.usage & nri::TextureUsageBits::SHADER_RESOURCE);
            assert(nriTextureDesc.format == nri::Format::RGBA16_SFLOAT);

            nvrhi::TextureDesc nvrhiTextureDesc{};
            nvrhiTextureDesc.dimension = nvrhi::TextureDimension::Texture2D;
            nvrhiTextureDesc.format = nvrhi::Format::RGBA16_FLOAT;
            nvrhiTextureDesc.width = nriTextureDesc.width;
            nvrhiTextureDesc.height = nriTextureDesc.height;
            nvrhiTextureDesc.isRenderTarget = true;
            nvrhiTextureDesc.isShaderResource = true;
            nvrhiTextureDesc.depth = nriTextureDesc.depth;
            nvrhiTextureDesc.mipLevels = nriTextureDesc.mipNum;
            nvrhiTextureDesc.arraySize = nriTextureDesc.layerNum;
            nvrhiTextureDesc.sampleCount = nriTextureDesc.sampleNum;

            auto nvrhiTexture = m_nvrhiDevice->createHandleForNativeTexture(
                nvrhi::ObjectTypes::D3D12_Resource, m_nriInterface.GetTextureNativeObject(*nriTexture),
                nvrhiTextureDesc);
            m_nvrhiSwapChainTextures.push_back(nvrhiTexture);
        }

        SOGE_INFO_LOG("Creating NVRHI framebuffer for swap chain textures...");
        nvrhi::FramebufferDesc framebufferDesc{};
        framebufferDesc.addColorAttachment(m_nvrhiSwapChainTextures[0]);

        m_nvrhiFramebuffer = m_nvrhiDevice->createFramebuffer(framebufferDesc);

        // TODO: move code below into pipeline class
        SOGE_INFO_LOG("Creating simple pipeline...");

        constexpr std::string_view shaderBinary = R"(
            struct VS_Input
            {
                float3 position : POSITION0;
                float4 color : COLOR0;
            };
            
            struct VS_Output
            {
                float4 position : SV_POSITION;
                float4 color : COLOR0;
            };
            
            VS_Output VSMain(VS_Input input)
            {
                VS_Output output = (VS_Output)0;
            
                output.position = float4(input.position, 1.0f);
                output.color = input.color;
            
                return output;
            }
            
            typedef VS_Output PS_Input;
            
            float4 PSMain(PS_Input input) : SV_Target
            {
                float4 color = input.color;
                return color;
            }
        )";
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

        nvrhi::ShaderDesc vertexShaderDesc{};
        vertexShaderDesc.shaderType = nvrhi::ShaderType::Vertex;
        vertexShaderDesc.debugName = "SOGE vertex shader";
        vertexShaderDesc.entryName = "VSMain";
        m_nvrhiVertexShader = m_nvrhiDevice->createShader(vertexShaderDesc, shaderBinary.data(), shaderBinary.size());

        nvrhi::ShaderDesc pixelShaderDesc{};
        pixelShaderDesc.shaderType = nvrhi::ShaderType::Pixel;
        pixelShaderDesc.debugName = "SOGE pixel shader";
        pixelShaderDesc.entryName = "PSMain";
        m_nvrhiPixelShader = m_nvrhiDevice->createShader(pixelShaderDesc, shaderBinary.data(), shaderBinary.size());

        m_nvrhiInputLayout = m_nvrhiDevice->createInputLayout(vertexAttributeDescArray.data(),
                                                              vertexAttributeDescArray.size(), m_nvrhiVertexShader);

        nvrhi::BindingLayoutDesc bindingLayoutDesc{};
        bindingLayoutDesc.visibility = nvrhi::ShaderType::All;
        m_nvrhiBindingLayout = m_nvrhiDevice->createBindingLayout(bindingLayoutDesc);

        nvrhi::GraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.inputLayout = m_nvrhiInputLayout;
        pipelineDesc.VS = m_nvrhiVertexShader;
        pipelineDesc.PS = m_nvrhiPixelShader;
        pipelineDesc.bindingLayouts = {m_nvrhiBindingLayout};
        pipelineDesc.renderState.depthStencilState.depthTestEnable = false;
        m_nvrhiGraphicsPipeline = m_nvrhiDevice->createGraphicsPipeline(pipelineDesc, m_nvrhiFramebuffer);
    }

    void D3D12GraphicsCore::Update(float aDeltaTime)
    {
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
