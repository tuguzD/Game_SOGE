#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"

#include "SOGE/Graphics/Exceptions/NRIException.hpp"

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
    D3D12GraphicsCore::D3D12GraphicsCore() : m_nriCallbackInterface(), m_nriAllocationCallbacks()
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
        NRIThrowIfFailed(nri::nriCreateDevice(deviceCreationDesc, m_nriDevice), "creating render device");

        SOGE_INFO_LOG("Retrieving NRI interfaces...");
        const auto nriCoreInterface = static_cast<nri::CoreInterface*>(&m_nriInterface);
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::CoreInterface), nriCoreInterface),
                         "retrieving NRI core interface");
        m_nriInterface.SetDeviceDebugName(*m_nriDevice, "SOGE D3D12 GPU device");

        const auto nriHelperInterface = static_cast<nri::HelperInterface*>(&m_nriInterface);
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::HelperInterface), nriHelperInterface),
                         "retrieving NRI helper interface");
        const auto nriStreamerInterface = static_cast<nri::StreamerInterface*>(&m_nriInterface);
        NRIThrowIfFailed(
            nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::StreamerInterface), nriStreamerInterface),
            "retrieving NRI streamer interface");
        const auto nriSwapChainInterface = static_cast<nri::SwapChainInterface*>(&m_nriInterface);
        NRIThrowIfFailed(
            nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::SwapChainInterface), nriSwapChainInterface),
            "retrieving NRI swap chain interface");

        SOGE_INFO_LOG("Creating graphics command queue...");
        D3D12_COMMAND_QUEUE_DESC graphicsCommandQueueDesc{};
        graphicsCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        graphicsCommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        graphicsCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        nvrhi::RefCountPtr<ID3D12CommandQueue> graphicsCommandQueue;
        const auto d3d12Device = static_cast<ID3D12Device*>(m_nriInterface.GetDeviceNativeObject(*m_nriDevice));
        NRIThrowIfFailed(ConvertResult(d3d12Device->CreateCommandQueue(&graphicsCommandQueueDesc,
                                                                       IID_PPV_ARGS(&graphicsCommandQueue))),
                         "creating graphics command queue");
        NRIThrowIfFailed(ConvertResult(graphicsCommandQueue->SetName(L"SOGE graphics command queue")),
                         "setting debug name for graphics command queue");

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

        SOGE_INFO_LOG("Destroying NVRHI render device...");
        m_nvrhiDevice = nullptr;

        if (m_nriDevice != nullptr)
        {
            SOGE_INFO_LOG("Destroying NRI render device...");
            nri::nriDestroyDevice(*m_nriDevice);
        }
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
