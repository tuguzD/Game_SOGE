#ifndef SOGE_GRAPHICS_GRAPHICSCORE_HPP
#define SOGE_GRAPHICS_GRAPHICSCORE_HPP

#include "SOGE/Event/EventModule.hpp"

namespace soge
{
    class GraphicsCore
    {
    private:
        eastl::reference_wrapper<EventModule> m_eventModule;

    public:
        explicit GraphicsCore(EventModule& aEventModule) noexcept;

        constexpr explicit GraphicsCore(const GraphicsCore&) noexcept = delete;
        constexpr GraphicsCore& operator=(const GraphicsCore&) noexcept = delete;

        constexpr explicit GraphicsCore(GraphicsCore&&) noexcept = default;
        constexpr GraphicsCore& operator=(GraphicsCore&&) noexcept = default;

        constexpr virtual ~GraphicsCore() noexcept = default;

        virtual void Update(float aDeltaTime) = 0;

    };
}

SOGE_DI_REGISTER_NS(soge, GraphicsCore, df::Abstract<GraphicsCore>)

#endif // SOGE_GRAPHICS_GRAPHICSCORE_HPP
