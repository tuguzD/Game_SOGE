#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"
#include "SOGE/Graphics/Exceptions/NRIException.hpp"

#include <Extensions/NRIDeviceCreation.h>
#include <nvrhi/d3d12.h>


namespace
{
    bool CheckResult(const nri::Result aResult, const eastl::string_view aContext = {})
    {
        switch (aResult)
        {
        case nri::Result::SUCCESS:
        case nri::Result::MAX_NUM: {
            return true;
        }
        case nri::Result::FAILURE: {
            aContext.empty() ? SOGE_ERROR_LOG("Render failure occurred...")
                             : SOGE_ERROR_LOG("Render failure occurred: {}", aContext.data());
            return false;
        }
        case nri::Result::INVALID_ARGUMENT: {
            aContext.empty() ? SOGE_ERROR_LOG("Render failure (invalid argument were passed)...")
                             : SOGE_ERROR_LOG("Render failure (invalid argument were passed): {}", aContext.data());
            return false;
        }
        case nri::Result::OUT_OF_MEMORY: {
            aContext.empty() ? SOGE_ERROR_LOG("Render failure (out of memory)...")
                             : SOGE_ERROR_LOG("Render failure (out of memory): {}", aContext.data());
            return false;
        }
        case nri::Result::UNSUPPORTED: {
            aContext.empty() ? SOGE_ERROR_LOG("Render failure (unsupported)...")
                             : SOGE_ERROR_LOG("Render failure (unsupported): {}", aContext.data());
            return false;
        }
        case nri::Result::DEVICE_LOST: {
            aContext.empty() ? SOGE_ERROR_LOG("Render failure (device lost)...")
                             : SOGE_ERROR_LOG("Render failure (device lost): {}", aContext.data());
            return false;
        }
        case nri::Result::OUT_OF_DATE: {
            aContext.empty() ? SOGE_ERROR_LOG("Render failure (swapchain is out of date)...")
                             : SOGE_ERROR_LOG("Render failure (swapchain is out of date): {}", aContext.data());
            return false;
        }
        }
        return false;
    }
}

namespace soge
{
    class D3D12GraphicsCore::MessageCallback final : public nvrhi::IMessageCallback
    {
    public:
        explicit MessageCallback() = default;

        MessageCallback(const MessageCallback&) = delete;
        MessageCallback& operator=(const MessageCallback&) = delete;

        MessageCallback(MessageCallback&&) = delete;
        MessageCallback& operator=(MessageCallback&&) = delete;

        ~MessageCallback() override = default;

        void message(const nvrhi::MessageSeverity aSeverity, const char* aMessageText) override
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
    };

    D3D12GraphicsCore::D3D12GraphicsCore() : m_messageCallback(CreateUnique<MessageCallback>())
    {
        SOGE_INFO_LOG("Creating render device...");

        m_nriCoreInterface      = new nri::CoreInterface();
        m_nriSwapChainInterface = new nri::SwapChainInterface();
        m_nriHelperInterface    = new nri::HelperInterface();
        m_nriStreamerInterface  = new nri::StreamerInterface();

        nri::AdapterDesc bestAdapterDesc = {};
        std::uint32_t adapterDescNum = 1;
        NRIThrowIfFailed(nri::nriEnumerateAdapters(&bestAdapterDesc, adapterDescNum), "Choosing device");
        SOGE_INFO_LOG(R"(Rendering device "{}" was chosen...)", bestAdapterDesc.name);

        nri::DeviceCreationDesc deviceCreationDesc = {};
        deviceCreationDesc.graphicsAPI = nri::GraphicsAPI::D3D12;
        deviceCreationDesc.enableGraphicsAPIValidation = true;
        deviceCreationDesc.enableNRIValidation = true;
        deviceCreationDesc.adapterDesc = &bestAdapterDesc;
        deviceCreationDesc.allocationCallbacks = m_allocationCallbacks;

        NRIThrowIfFailed(nri::nriCreateDevice(
            deviceCreationDesc, m_device), "Creating render device");
        NRIThrowIfFailed(nri::nriGetInterface(
            *m_device, NRI_INTERFACE(nri::CoreInterface), m_nriCoreInterface));
        NRIThrowIfFailed(nri::nriGetInterface(
            *m_device, NRI_INTERFACE(nri::SwapChainInterface), m_nriSwapChainInterface));
        NRIThrowIfFailed(nri::nriGetInterface(*m_device, NRI_INTERFACE(nri::HelperInterface), m_nriHelperInterface));
        NRIThrowIfFailed(nri::nriGetInterface(*m_device, NRI_INTERFACE(nri::StreamerInterface), m_nriStreamerInterface));

        NRIThrowIfFailed(m_nriCoreInterface->GetCommandQueue(*m_device, nri::CommandQueueType::GRAPHICS, m_commandQueue));
        NRIThrowIfFailed(m_nriCoreInterface->CreateFence(*m_device, 0, m_frameFence), "Creating frame fence");

        // TODO: uncomment to get linker error
         //const nvrhi::d3d12::DeviceDesc deviceDesc{
         //    .errorCB = m_messageCallback.get(),
         //    .pDevice = nri::nriGetInterface(),
         //    .pGraphicsCommandQueue = 
         //};
         //m_deviceWrapper = nvrhi::d3d12::createDevice(deviceDesc);
    }

    D3D12GraphicsCore::~D3D12GraphicsCore()
    {
        if (m_device != nullptr)
        {
            SOGE_INFO_LOG("Destroying render device...");
            m_nriCoreInterface->DestroyFence(*m_frameFence);
            //m_nriSwapChainInterface->DestroySwapChain(*m_swapChain); // Not initialize SwapChain for now...

            nri::nriDestroyDevice(*m_device);
        }
    }

    void D3D12GraphicsCore::Update(float aDeltaTime)
    {
    }
}
