#ifndef SOGE_GRAPHICS_RENDERGRAPH_HPP
#define SOGE_GRAPHICS_RENDERGRAPH_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"


namespace soge
{
    class RenderGraph
    {
    public:
        constexpr explicit RenderGraph(GraphicsCore& aCore) noexcept;

        constexpr RenderGraph(const RenderGraph&) = delete;
        constexpr RenderGraph& operator=(const RenderGraph&) = delete;

        constexpr RenderGraph(RenderGraph&&) noexcept = default;
        constexpr RenderGraph& operator=(RenderGraph&&) noexcept = default;

        constexpr virtual ~RenderGraph() = default;

        constexpr virtual void Execute(float aDeltaTime) = 0;
    };

    constexpr RenderGraph::RenderGraph(GraphicsCore&) noexcept
    {
    }
}

SOGE_DI_REGISTER_NS(soge, RenderGraph, df::Abstract<RenderGraph>)

#endif // SOGE_GRAPHICS_RENDERGRAPH_HPP
