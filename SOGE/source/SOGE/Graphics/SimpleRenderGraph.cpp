#include "sogepch.hpp"

#include "SOGE/Graphics/SimpleRenderGraph.hpp"


namespace soge
{
    SimpleRenderGraph::SimpleRenderGraph(GraphicsCore& aCore, TriangleGraphicsPipeline& aTrianglePipeline,
                                         GeometryGraphicsPipeline& aGeometryPipeline,
                                         LightGraphicsPipeline& aLightPipeline)
        : RenderGraph{aCore}, m_core{aCore}, m_trianglePipeline{aTrianglePipeline},
          m_geometryPipeline{aGeometryPipeline}, m_lightPipeline{aLightPipeline}
    {
    }

    void SimpleRenderGraph::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, const Entities aEntities)
    {
        const auto geometryCommandLists = m_geometryPipeline.get().Execute(aViewport, aCamera, aEntities);
        m_core.get().ExecuteCommandLists(geometryCommandLists, nvrhi::CommandQueue::Graphics);

        // const auto triangleCommandLists = m_trianglePipeline.get().Execute(aViewport, aCamera, aEntities);
        // m_core.get().ExecuteCommandLists(triangleCommandLists, nvrhi::CommandQueue::Graphics);

        const auto lightCommandLists = m_lightPipeline.get().Execute(aViewport, aCamera, aEntities);
        m_core.get().ExecuteCommandLists(lightCommandLists, nvrhi::CommandQueue::Graphics);
    }
}
