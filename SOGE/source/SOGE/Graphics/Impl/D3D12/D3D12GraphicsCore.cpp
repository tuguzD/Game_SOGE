#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"

#include "SOGE/Graphics/Exceptions/NRIException.hpp"
#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"
#include "SOGE/Graphics/GraphicsModule.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"
#include "SOGE/Window/Window.hpp"

#include <Extensions/NRIWrapperD3D12.h>

#include <nvrhi/utils.h>
#ifdef SOGE_DEBUG
#include <nvrhi/validation.h>
#endif


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
    D3D12GraphicsCore::D3D12GraphicsCore() : m_nriCallbackInterface{}, m_nriAllocationCallbacks{}, m_totalFrameCount{}
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
        m_graphicsPipeline = eastl::nullopt;

        if (!m_nvrhiFramebuffers.empty())
        {
            SOGE_INFO_LOG("Destroying NVRHI framebuffers...");
            m_nvrhiFramebuffers.clear();
        }

        m_swapChain = eastl::nullopt;
    }

    void D3D12GraphicsCore::DestroyDevice()
    {
        if (m_nvrhiDevice != nullptr)
        {
            SOGE_INFO_LOG("Destroying NVRHI render device...");
            m_nvrhiDevice = nullptr;
        }

        if (m_nriDevice != nullptr)
        {
            SOGE_INFO_LOG("Destroying NRI render device wrapper...");
            nri::nriDestroyDevice(*m_nriDevice);
            m_nriDevice = nullptr;
            m_nriGraphicsCommandQueue = nullptr;
        }

        if (m_nriInitDevice != nullptr)
        {
            SOGE_INFO_LOG("Destroying NRI render device...");
            nri::nriDestroyDevice(*m_nriInitDevice);
            m_nriInitDevice = nullptr;
        }
    }

    void D3D12GraphicsCore::SetRenderTarget(const Window& aWindow)
    {
        DestroySwapChain();

        m_swapChain.emplace(aWindow, *this);

        SOGE_INFO_LOG("Creating NVRHI depth texture...");
        nvrhi::TextureDesc nvrhiDepthTextureDesc{};
        nvrhiDepthTextureDesc.dimension = nvrhi::TextureDimension::Texture2D;
        nvrhiDepthTextureDesc.width = aWindow.GetWidth();
        nvrhiDepthTextureDesc.height = aWindow.GetHeight();
        nvrhiDepthTextureDesc.isRenderTarget = true;
        nvrhiDepthTextureDesc.isShaderResource = true;
        nvrhiDepthTextureDesc.isTypeless = true;
        nvrhiDepthTextureDesc.mipLevels = 1;
        nvrhiDepthTextureDesc.useClearValue = true;
        nvrhiDepthTextureDesc.clearValue = nvrhi::Color{1.0f, 0.0f, 0.0f, 0.0f};
        nvrhiDepthTextureDesc.initialState = nvrhi::ResourceStates::DepthWrite;
        nvrhiDepthTextureDesc.keepInitialState = true;
        nvrhiDepthTextureDesc.debugName = "SOGE depth texture";

        const nvrhi::FormatSupport requiredDepthFeatures =
            nvrhi::FormatSupport::Texture | nvrhi::FormatSupport::DepthStencil | nvrhi::FormatSupport::ShaderLoad;
        constexpr std::array requestedDepthFormats{
            nvrhi::Format::D24S8,
            nvrhi::Format::D32S8,
            nvrhi::Format::D32,
            nvrhi::Format::D16,
        };
        nvrhiDepthTextureDesc.format = nvrhi::utils::ChooseFormat(
            m_nvrhiDevice, requiredDepthFeatures, requestedDepthFormats.data(), requestedDepthFormats.size());

        const nvrhi::TextureHandle nvrhiDepthTexture = m_nvrhiDevice->createTexture(nvrhiDepthTextureDesc);

        const auto swapChainTextures = m_swapChain->GetTextures();
        m_nvrhiFramebuffers.reserve(swapChainTextures.size());
        for (std::size_t index = 0; index < swapChainTextures.size(); index++)
        {
            nvrhi::ITexture* nvrhiColorTexture = &swapChainTextures[index].get();

            SOGE_INFO_LOG("Creating NVRHI framebuffer (frame {})...", index);
            nvrhi::FramebufferDesc framebufferDesc{};
            framebufferDesc.addColorAttachment(nvrhiColorTexture);
            framebufferDesc.setDepthAttachment(nvrhiDepthTexture);

            nvrhi::FramebufferHandle nvrhiFramebuffer = m_nvrhiDevice->createFramebuffer(framebufferDesc);
            m_nvrhiFramebuffers.push_back(nvrhiFramebuffer);
        }

        m_graphicsPipeline.emplace(*this);
    }

    void D3D12GraphicsCore::Update(float aDeltaTime)
    {
        SOGE_INFO_LOG("Rendering {} frame with input delta time of {}...", m_totalFrameCount, aDeltaTime);

        m_swapChain->WaitForPresent();
        m_nvrhiDevice->runGarbageCollection();

        const auto currentFrameIndex = m_swapChain->GetCurrentTextureIndex();
        const auto currentFramebuffer = m_nvrhiFramebuffers[currentFrameIndex];

        nvrhi::CommandListParameters commandListDesc{};
        commandListDesc.enableImmediateExecution = false;

        const nvrhi::CommandListHandle clearCommandList = m_nvrhiDevice->createCommandList(commandListDesc);
        {
            GraphicsCommandListGuard commandList{*clearCommandList};

            const nvrhi::FramebufferDesc& framebufferDesc = currentFramebuffer->getDesc();
            for (std::uint32_t index = 0; index < framebufferDesc.colorAttachments.size(); index++)
            {
                nvrhi::utils::ClearColorAttachment(commandList, currentFramebuffer, index, nvrhi::Color{});
            }
            nvrhi::utils::ClearDepthStencilAttachment(commandList, currentFramebuffer, 1.0f, 0);
        }
        const auto drawCommandLists = m_graphicsPipeline->Update(aDeltaTime);

        m_commandLists.reserve(drawCommandLists.size() + 1);
        {
            m_commandLists.emplace_back(clearCommandList);
            for (const auto& drawCommandList : drawCommandLists)
            {
                m_commandLists.emplace_back(&drawCommandList.get());
            }
        }
        m_nvrhiDevice->executeCommandLists(m_commandLists.data(), m_commandLists.size(), nvrhi::CommandQueue::Graphics);
        m_commandLists.clear();

        m_swapChain->Present();
        m_totalFrameCount++;
    }

    nvrhi::IDevice& D3D12GraphicsCore::GetRawDevice()
    {
        return *m_nvrhiDevice;
    }

    eastl::string_view D3D12GraphicsCore::GetCompiledShaderExtension() const
    {
        return SOGE_STRINGIZE(SOGE_GRAPHICS_COMPILED_SHADER_EXTENSION_D3D12);
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
