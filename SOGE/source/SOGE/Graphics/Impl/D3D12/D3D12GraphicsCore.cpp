#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"

#include "SOGE/Graphics/Exceptions/NRIException.hpp"

#include <nvrhi/validation.h>


namespace soge
{
    D3D12GraphicsCore::D3D12GraphicsCore() : m_swapChain(nullptr)
    {
        SOGE_INFO_LOG("Creating D3D12 render backend...");

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

        SOGE_INFO_LOG("Retrieving command queue...");
        NRIThrowIfFailed(m_nriInterface.GetCommandQueue(*m_device, nri::CommandQueueType::GRAPHICS, m_commandQueue),
                         "retrieving command queue");
        m_nriInterface.SetCommandQueueDebugName(*m_commandQueue, "SOGE graphics command queue");

        SOGE_INFO_LOG("Creating frame fence...");
        NRIThrowIfFailed(m_nriInterface.CreateFence(*m_device, 0, m_frameFence), "creating frame fence");
        m_nriInterface.SetFenceDebugName(*m_frameFence, "SOGE graphics frame fence");

        SOGE_INFO_LOG("Creating NVRHI device wrapper...");
        const nvrhi::d3d12::DeviceDesc deviceDesc{
            .errorCB = &m_messageCallback,
            .pDevice = static_cast<ID3D12Device*>(m_nriInterface.GetDeviceNativeObject(*m_device)),
            // .pGraphicsCommandQueue = static_cast<CommandQueueD3D12*>(m_commandQueue),
        };
        m_deviceWrapper = nvrhi::d3d12::createDevice(deviceDesc);
        m_deviceWrapper = nvrhi::validation::createValidationLayer(m_deviceWrapper);
    }

    D3D12GraphicsCore::~D3D12GraphicsCore()
    {
        SOGE_INFO_LOG("Destroying D3D12 render backend...");

        SOGE_INFO_LOG("Destroying NVRHI device wrapper...");
        m_deviceWrapper = nullptr;

        if (m_commandQueue != nullptr)
        {
            SOGE_INFO_LOG("Waiting for command queue to be idle...");
            NRIThrowIfFailed(m_nriInterface.WaitForIdle(*m_commandQueue), "waiting for command queue to be idle");
        }

        // m_nriSwapChainInterface->DestroySwapChain(*m_swapChain); // Do not initialize swapchain for now...

        if (m_frameFence != nullptr)
        {
            SOGE_INFO_LOG("Destroying frame fence...");
            m_nriInterface.DestroyFence(*m_frameFence);
        }

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
        Logger::EngineLog(level, "{}", aMessageText);
    }
}
