#ifndef SOGE_GRAPHICS_IMPL_NVDA_NVDAGRAPHICSCORE_HPP
#define SOGE_GRAPHICS_IMPL_NVDA_NVDAGRAPHICSCORE_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/System/Memory.hpp"

#include <NRI.h>
#include <nvrhi/nvrhi.h>


namespace soge
{
    class NVDAGraphicsCore : public GraphicsCore
    {
    private:
        class MessageCallback;

        nri::Device* m_device;

        UniquePtr<MessageCallback> m_messageCallback;
        nvrhi::DeviceHandle m_deviceWrapper;

    public:
        explicit NVDAGraphicsCore();

        explicit NVDAGraphicsCore(const NVDAGraphicsCore&) = delete;
        NVDAGraphicsCore& operator=(const NVDAGraphicsCore&) = delete;

        explicit NVDAGraphicsCore(NVDAGraphicsCore&&) noexcept = default;
        NVDAGraphicsCore& operator=(NVDAGraphicsCore&&) noexcept = default;

        ~NVDAGraphicsCore() override;

        void Update(float aDeltaTime) override;
    };

    using ImplGraphicsCore = NVDAGraphicsCore;
}

SOGE_DI_REGISTER_NS(soge, NVDAGraphicsCore, df::Single<NVDAGraphicsCore>, tag::Overrides<GraphicsCore>)

#endif // SOGE_GRAPHICS_IMPL_NVDA_NVDAGRAPHICSCORE_HPP
