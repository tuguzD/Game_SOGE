#ifndef SOGE_GRAPHICS_SIMPLERENDERGRAPH_HPP
#define SOGE_GRAPHICS_SIMPLERENDERGRAPH_HPP

#include "SOGE/Graphics/GeometryGraphicsPipeline.hpp"
#include "SOGE/Graphics/LightGraphicsPipeline.hpp"
#include "SOGE/Graphics/RenderGraph.hpp"
#include "SOGE/Graphics/TriangleGraphicsPipeline.hpp"


namespace soge
{
    class SimpleRenderGraph : public RenderGraph
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;

        eastl::reference_wrapper<TriangleGraphicsPipeline> m_trianglePipeline;
        eastl::reference_wrapper<GeometryGraphicsPipeline> m_geometryPipeline;
        eastl::reference_wrapper<LightGraphicsPipeline> m_lightPipeline;

    public:
        explicit SimpleRenderGraph(GraphicsCore& aCore, TriangleGraphicsPipeline& aTrianglePipeline,
                                   GeometryGraphicsPipeline& aGeometryPipeline, LightGraphicsPipeline& aLightPipeline);

        void Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entities aEntities) override;
    };
}

SOGE_DI_REGISTER_NS(soge, SimpleRenderGraph,
                    df::Single<SimpleRenderGraph, GraphicsCore, TriangleGraphicsPipeline, GeometryGraphicsPipeline,
                               LightGraphicsPipeline>,
                    tag::Overrides<SimpleRenderGraph, RenderGraph>)

#endif // SOGE_GRAPHICS_SIMPLERENDERGRAPH_HPP
