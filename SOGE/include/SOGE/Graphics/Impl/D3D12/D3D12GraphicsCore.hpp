#ifndef SOGE_GRAPHICS_IMPL_D3D12_NVDAGRAPHICSCORE_HPP
#define SOGE_GRAPHICS_IMPL_D3D12_NVDAGRAPHICSCORE_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/System/Memory.hpp"

#include <NRI.h>
#include <Extensions/NRIHelper.h>
#include <Extensions/NRIStreamer.h>
#include <Extensions/NRISwapChain.h>
#include <Extensions/NRIDeviceCreation.h>

#include <nvrhi/nvrhi.h>
#include <nvrhi/d3d12.h>

namespace soge
{
    class D3D12GraphicsCore : public GraphicsCore
    {
    private:
        class MessageCallback;

        nri::CoreInterface* m_nriCoreInterface;
        nri::SwapChainInterface* m_nriSwapChainInterface;
        nri::HelperInterface* m_nriHelperInterface;
        nri::StreamerInterface* m_nriStreamerInterface;

        nri::Device* m_device;
        nri::SwapChain* m_swapChain;
        nri::CommandQueue* m_commandQueue;
        nri::Fence* m_frameFence;

        nri::AllocationCallbacks m_allocationCallbacks = {};
        UniquePtr<MessageCallback> m_messageCallback;

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

#endif // SOGE_GRAPHICS_IMPL_D3D12_NVDAGRAPHICSCORE_HPP
