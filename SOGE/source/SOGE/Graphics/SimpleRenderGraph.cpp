#include "sogepch.hpp"

#include "SOGE/Graphics/SimpleRenderGraph.hpp"


namespace soge
{
    SimpleRenderGraph::SimpleRenderGraph(GraphicsCore& aCore, FinalGraphicsRenderPass& aFinalRenderPass,
                                         GeometryGraphicsRenderPass& aGeometryRenderPass,
                                         TriangleGraphicsPipeline& aTrianglePipeline)
        : RenderGraph{aCore}, m_core{aCore}, m_finalRenderPass{aFinalRenderPass},
          m_geometryRenderPass{aGeometryRenderPass}, m_trianglePipeline{aTrianglePipeline}
    {
    }

    void SimpleRenderGraph::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, const Entities aEntities)
    {
        const auto triangleCommandLists = m_trianglePipeline.get().Execute(aViewport, aCamera, aEntities);
        m_core.get().ExecuteCommandLists(triangleCommandLists, nvrhi::CommandQueue::Graphics);
    }
}
