#include "sogepch.hpp"

#include "SOGE/Graphics/Exceptions/NRIException.hpp"
#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"


namespace soge
{
    D3D12GraphicsCore::D3D12GraphicsCore()
    {
        SOGE_INFO_LOG("Creating render device...");

        nri::AdapterDesc bestAdapterDesc{};
        std::uint32_t adapterDescNum = 1;
        NRIThrowIfFailed(nri::nriEnumerateAdapters(&bestAdapterDesc, adapterDescNum), "Choosing device");
        SOGE_INFO_LOG(R"(Rendering device "{}" was chosen...)", bestAdapterDesc.name);

        nri::DeviceCreationDesc deviceCreationDesc = {};
        deviceCreationDesc.graphicsAPI = nri::GraphicsAPI::D3D12;
        deviceCreationDesc.enableGraphicsAPIValidation = true;
        deviceCreationDesc.enableNRIValidation = true;
        deviceCreationDesc.adapterDesc = &bestAdapterDesc;
        deviceCreationDesc.allocationCallbacks = m_allocationCallbacks;
        NRIThrowIfFailed(nri::nriCreateDevice(deviceCreationDesc, m_device), "Creating render device");

        const auto coreInterface = static_cast<nri::CoreInterface*>(&m_nriInterface);
        NRIThrowIfFailed(nri::nriGetInterface(*m_device, NRI_INTERFACE(nri::CoreInterface), coreInterface));
        const auto helperInterface = static_cast<nri::HelperInterface*>(&m_nriInterface);
        NRIThrowIfFailed(nri::nriGetInterface(*m_device, NRI_INTERFACE(nri::HelperInterface), helperInterface));
        const auto streamerInterface = static_cast<nri::StreamerInterface*>(&m_nriInterface);
        NRIThrowIfFailed(nri::nriGetInterface(*m_device, NRI_INTERFACE(nri::StreamerInterface), streamerInterface));
        const auto swapChainInterface = static_cast<nri::SwapChainInterface*>(&m_nriInterface);
        NRIThrowIfFailed(nri::nriGetInterface(*m_device, NRI_INTERFACE(nri::SwapChainInterface), swapChainInterface));

        NRIThrowIfFailed(m_nriInterface.GetCommandQueue(*m_device, nri::CommandQueueType::GRAPHICS, m_commandQueue));
        NRIThrowIfFailed(m_nriInterface.CreateFence(*m_device, 0, m_frameFence), "Creating frame fence");

        m_swapChain = nullptr; // Not initialize SwapChain for now...

        // TODO: uncomment to get linker error
        // const nvrhi::d3d12::DeviceDesc deviceDesc{
        //     .errorCB = m_messageCallback.get(),
        //     .pDevice = nri::nriGetInterface(),
        //     .pGraphicsCommandQueue =
        // };
        // m_deviceWrapper = nvrhi::d3d12::createDevice(deviceDesc);
    }

    D3D12GraphicsCore::~D3D12GraphicsCore()
    {
        if (m_device != nullptr)
        {
            SOGE_INFO_LOG("Destroying render device...");
            m_nriInterface.DestroyFence(*m_frameFence);
            // m_nriSwapChainInterface->DestroySwapChain(*m_swapChain); // Not initialize SwapChain for now...

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
