#ifndef SOGE_GRAPHICS_IMPL_NVDA_NVDAGRAPHICSCORE_HPP
#define SOGE_GRAPHICS_IMPL_NVDA_NVDAGRAPHICSCORE_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"


namespace soge
{
    class NVDAGraphicsCore : public GraphicsCore
    {
    public:
        explicit NVDAGraphicsCore(EventModule& aEventModule);
        ~NVDAGraphicsCore() = default;

        void Update(float aDeltaTime) override;

    };

    using ImplGraphicsCore = NVDAGraphicsCore;
}

SOGE_DI_REGISTER_NS(soge, NVDAGraphicsCore, df::Single<NVDAGraphicsCore, EventModule>, tag::Overrides<GraphicsCore>)

#endif // !SOGE_GRAPHICS_IMPL_NVDA_NVDAGRAPHICSCORE_HPP
