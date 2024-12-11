#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/NVDA/NVDAGraphicsCore.hpp"

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
    class NVDAGraphicsCore::MessageCallback final : public nvrhi::IMessageCallback
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

    NVDAGraphicsCore::NVDAGraphicsCore() : m_messageCallback(CreateUnique<MessageCallback>())
    {
        SOGE_INFO_LOG("Creating render device...");

        nri::AdapterDesc bestAdapterDesc;
        std::uint32_t adapterDescNum = 1;
        if (const auto result = nri::nriEnumerateAdapters(&bestAdapterDesc, adapterDescNum);
            !CheckResult(result, "choosing the best device out there"))
        {
            return;
        }
        SOGE_INFO_LOG(R"(Rendering device "{}" was chosen...)", bestAdapterDesc.name);

        constexpr nri::DeviceCreationDesc deviceCreationDesc{
            .graphicsAPI = nri::GraphicsAPI::D3D12,
            .enableNRIValidation = true,
            .enableGraphicsAPIValidation = true,
        }; // NOLINT(clang-diagnostic-missing-designated-field-initializers)
        if (const auto result = nri::nriCreateDevice(deviceCreationDesc, m_device);
            !CheckResult(result, "creating rendering device"))
        {
            return;
        }

        // TODO: uncomment to get linker error
        // const nvrhi::d3d12::DeviceDesc deviceDesc{
        //     .errorCB = m_messageCallback.get(),
        // };
        // m_deviceWrapper = nvrhi::d3d12::createDevice(deviceDesc);
    }

    NVDAGraphicsCore::~NVDAGraphicsCore()
    {
        if (m_device != nullptr)
        {
            SOGE_INFO_LOG("Destroying render device...");
            nri::nriDestroyDevice(*m_device);
        }
    }

    void NVDAGraphicsCore::Update(float aDeltaTime)
    {
    }
}
