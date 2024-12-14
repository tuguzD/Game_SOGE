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
        class MessageCallback final : public nvrhi::IMessageCallback
        {
        public:
            explicit MessageCallback() = default;

            explicit MessageCallback(const MessageCallback&) = delete;
            MessageCallback& operator=(const MessageCallback&) = delete;

            explicit MessageCallback(MessageCallback&&) noexcept;
            MessageCallback& operator=(MessageCallback&&) noexcept;

            ~MessageCallback() override = default;

            void message(nvrhi::MessageSeverity aSeverity, const char* aMessageText) override;
        };

        class NRIInterface final : public nri::CoreInterface,
                                   public nri::HelperInterface,
                                   public nri::StreamerInterface,
                                   public nri::SwapChainInterface
        {
        };

        nri::Device* m_device;
        NRIInterface m_nriInterface;
        nri::AllocationCallbacks m_allocationCallbacks{};

        nri::CommandQueue* m_commandQueue;
        nri::Fence* m_frameFence;
        nri::SwapChain* m_swapChain;

        MessageCallback m_messageCallback;
        nvrhi::DeviceHandle m_deviceWrapper;

    public:
        explicit D3D12GraphicsCore();

        explicit D3D12GraphicsCore(const D3D12GraphicsCore&) = delete;
        D3D12GraphicsCore& operator=(const D3D12GraphicsCore&) = delete;

        explicit D3D12GraphicsCore(D3D12GraphicsCore&&) noexcept = default;
        D3D12GraphicsCore& operator=(D3D12GraphicsCore&&) noexcept = default;

        ~D3D12GraphicsCore() override;

        void Update(float aDeltaTime) override;
    };

    using ImplGraphicsCore = D3D12GraphicsCore;
}

SOGE_DI_REGISTER_NS(soge, D3D12GraphicsCore, df::Single<D3D12GraphicsCore>, tag::Overrides<GraphicsCore>)

#endif // SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSCORE_HPP
