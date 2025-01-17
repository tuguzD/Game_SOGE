#ifndef SOGE_GRAPHICS_DEFERRED_SIMPLERENDERGRAPH_HPP
#define SOGE_GRAPHICS_DEFERRED_SIMPLERENDERGRAPH_HPP

#include "SOGE/Graphics/Deferred/GeometryGraphicsPipeline.hpp"
#include "SOGE/Graphics/Deferred/LightGraphicsPipeline.hpp"
#include "SOGE/Graphics/RenderGraph.hpp"


namespace soge
{
    class DeferredRenderGraph : public RenderGraph
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;

        eastl::reference_wrapper<GeometryGraphicsRenderPass> m_geometryPass;
        eastl::reference_wrapper<FinalGraphicsRenderPass> m_finalPass;

        eastl::reference_wrapper<GeometryGraphicsPipeline> m_geometryPipeline;
        eastl::reference_wrapper<LightGraphicsPipeline> m_lightPipeline;

    public:
        explicit DeferredRenderGraph(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryPass,
                                     FinalGraphicsRenderPass& aFinalPass, GeometryGraphicsPipeline& aGeometryPipeline,
                                     LightGraphicsPipeline& aLightPipeline);

        void Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entities aEntities) override;
    };
}

SOGE_DI_REGISTER_NS(soge, DeferredRenderGraph,
                    df::Single<DeferredRenderGraph, GraphicsCore, GeometryGraphicsRenderPass, FinalGraphicsRenderPass,
                               GeometryGraphicsPipeline, LightGraphicsPipeline>,
                    tag::Overrides<DeferredRenderGraph, RenderGraph>)

#endif // SOGE_GRAPHICS_DEFERRED_SIMPLERENDERGRAPH_HPP
