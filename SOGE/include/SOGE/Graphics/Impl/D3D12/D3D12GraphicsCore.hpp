#ifndef SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSCORE_HPP
#define SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSCORE_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"

#include <NRI.h>

#include <Extensions/NRIDeviceCreation.h>
#include <Extensions/NRIHelper.h>
#include <Extensions/NRIStreamer.h>
#include <Extensions/NRISwapChain.h>

#include <nvrhi/d3d12.h>


namespace soge
{
    class D3D12GraphicsCore : public GraphicsCore
    {
    private:
        class NvrhiMessageCallback final : public nvrhi::IMessageCallback
        {
        public:
            explicit NvrhiMessageCallback() = default;

            explicit NvrhiMessageCallback(const NvrhiMessageCallback&) = delete;
            NvrhiMessageCallback& operator=(const NvrhiMessageCallback&) = delete;

            explicit NvrhiMessageCallback(NvrhiMessageCallback&&) noexcept;
            NvrhiMessageCallback& operator=(NvrhiMessageCallback&&) noexcept;

            ~NvrhiMessageCallback() override = default;

            void message(nvrhi::MessageSeverity aSeverity, const char* aMessageText) override;
        };

        class NriInterface final : public nri::CoreInterface,
                                   public nri::HelperInterface,
                                   public nri::StreamerInterface,
                                   public nri::SwapChainInterface
        {
        };

        static void NriMessageCallback(nri::Message aMessageType, const char* aFile, std::uint32_t aLine,
                                       const char* aMessage, void* aUserArg);

        nri::Device* m_nriDevice;
        NriInterface m_nriInterface;
        nri::CallbackInterface m_nriCallbackInterface;
        nri::AllocationCallbacks m_nriAllocationCallbacks;

        NvrhiMessageCallback m_nvrhiMessageCallback;
        nvrhi::DeviceHandle m_nvrhiDevice;

    public:
        explicit D3D12GraphicsCore();

        explicit D3D12GraphicsCore(const D3D12GraphicsCore&) = delete;
        D3D12GraphicsCore& operator=(const D3D12GraphicsCore&) = delete;

        explicit D3D12GraphicsCore(D3D12GraphicsCore&&) noexcept = delete;
        D3D12GraphicsCore& operator=(D3D12GraphicsCore&&) noexcept = delete;

        ~D3D12GraphicsCore() override;

        void Update(float aDeltaTime) override;
    };

    using ImplGraphicsCore = D3D12GraphicsCore;
}

SOGE_DI_REGISTER_NS(soge, D3D12GraphicsCore, df::Single<D3D12GraphicsCore>, tag::Overrides<GraphicsCore>)

#endif // SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSCORE_HPP
