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
    D3D12GraphicsCore::D3D12GraphicsCore() : m_callbackInterface(), m_allocationCallbacks()
    {
        SOGE_INFO_LOG("Creating D3D12 render backend...");
        m_callbackInterface.MessageCallback = NRIMessageCallback;

        SOGE_INFO_LOG("Choosing best rendering device...");
        nri::AdapterDesc bestAdapterDesc{};
        std::uint32_t adapterDescNum = 1;
        NRIThrowIfFailed(nri::nriEnumerateAdapters(&bestAdapterDesc, adapterDescNum), "choosing best device");
        SOGE_INFO_LOG(R"(Rendering device "{}" was chosen...)", bestAdapterDesc.name);

        SOGE_INFO_LOG("Creating render device...");
        nri::DeviceCreationDesc deviceCreationDesc{};
        deviceCreationDesc.graphicsAPI = nri::GraphicsAPI::D3D12;
        deviceCreationDesc.enableGraphicsAPIValidation = true;
        deviceCreationDesc.enableNRIValidation = true;
        deviceCreationDesc.adapterDesc = &bestAdapterDesc;
        deviceCreationDesc.callbackInterface = m_callbackInterface;
        deviceCreationDesc.allocationCallbacks = m_allocationCallbacks;
        NRIThrowIfFailed(nri::nriCreateDevice(deviceCreationDesc, m_device), "creating render device");

        SOGE_INFO_LOG("Retrieving NRI interfaces...");
        const auto coreInterface = static_cast<nri::CoreInterface*>(&m_nriInterface);
        NRIThrowIfFailed(nri::nriGetInterface(*m_device, NRI_INTERFACE(nri::CoreInterface), coreInterface));
        m_nriInterface.SetDeviceDebugName(*m_device, "SOGE D3D12 GPU device");

        const auto helperInterface = static_cast<nri::HelperInterface*>(&m_nriInterface);
        NRIThrowIfFailed(nri::nriGetInterface(*m_device, NRI_INTERFACE(nri::HelperInterface), helperInterface));
        const auto streamerInterface = static_cast<nri::StreamerInterface*>(&m_nriInterface);
        NRIThrowIfFailed(nri::nriGetInterface(*m_device, NRI_INTERFACE(nri::StreamerInterface), streamerInterface));
        const auto swapChainInterface = static_cast<nri::SwapChainInterface*>(&m_nriInterface);
        NRIThrowIfFailed(nri::nriGetInterface(*m_device, NRI_INTERFACE(nri::SwapChainInterface), swapChainInterface));

        SOGE_INFO_LOG("Creating graphics command queue...");
        D3D12_COMMAND_QUEUE_DESC graphicsCommandQueueDesc{};
        graphicsCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        graphicsCommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        graphicsCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        nvrhi::RefCountPtr<ID3D12CommandQueue> graphicsCommandQueue;
        const auto d3d12Device = static_cast<ID3D12Device*>(m_nriInterface.GetDeviceNativeObject(*m_device));
        NRIThrowIfFailed(ConvertResult(d3d12Device->CreateCommandQueue(&graphicsCommandQueueDesc,
                                                                       IID_PPV_ARGS(&graphicsCommandQueue))),
                         "creating graphics command queue");
        NRIThrowIfFailed(ConvertResult(graphicsCommandQueue->SetName(L"SOGE graphics command queue")),
                         "setting debug name for graphics command queue");

        SOGE_INFO_LOG("Creating NVRHI device wrapper...");
        const nvrhi::d3d12::DeviceDesc deviceDesc{
            .errorCB = &m_messageCallback,
            .pDevice = d3d12Device,
            .pGraphicsCommandQueue = graphicsCommandQueue.Detach(),
        };
        m_deviceWrapper = nvrhi::d3d12::createDevice(deviceDesc);
        m_deviceWrapper = nvrhi::validation::createValidationLayer(m_deviceWrapper);
    }

    D3D12GraphicsCore::~D3D12GraphicsCore()
    {
        SOGE_INFO_LOG("Destroying D3D12 render backend...");

        SOGE_INFO_LOG("Destroying NVRHI device wrapper...");
        m_deviceWrapper = nullptr;

        if (m_device != nullptr)
        {
            SOGE_INFO_LOG("Destroying render device...");
            nri::nriDestroyDevice(*m_device);
        }
    }

    void D3D12GraphicsCore::Update(float aDeltaTime)
    {
    }

    D3D12GraphicsCore::MessageCallback::MessageCallback(MessageCallback&&) noexcept
    {
    }

    auto D3D12GraphicsCore::MessageCallback::operator=(MessageCallback&&) noexcept -> MessageCallback&
    {
        return *this;
    }

    void D3D12GraphicsCore::MessageCallback::message(const nvrhi::MessageSeverity aSeverity, const char* aMessageText)
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

    void D3D12GraphicsCore::NRIMessageCallback(const nri::Message aMessageType, const char* aFile, std::uint32_t aLine,
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
