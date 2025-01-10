#include "sogepch.hpp"

#include "SOGE/Graphics/SimpleRenderGraph.hpp"

#include "SOGE/Graphics/TriangleGraphicsPipeline.hpp"


namespace soge
{
    SimpleRenderGraph::SimpleRenderGraph(GraphicsCore& aCore, FinalGraphicsRenderPass& aFinalRenderPass,
                                         TriangleGraphicsPipeline& aTrianglePipeline)
        : RenderGraph{aCore}, m_core{aCore}, m_finalRenderPass{aFinalRenderPass}, m_trianglePipeline{aTrianglePipeline}
    {
    }

    void SimpleRenderGraph::Execute(const float aDeltaTime)
    {
        nvrhi::IDevice& device = m_core.get().GetRawDevice();

        const auto clearCommandList = m_finalRenderPass.get().CreateClearCommandList();
        const auto drawCommandLists = m_trianglePipeline.get().Update(aDeltaTime);

        m_commandLists.reserve(drawCommandLists.size() + 1);
        {
            m_commandLists.emplace_back(clearCommandList);
            for (const auto& drawCommandList : drawCommandLists)
            {
                m_commandLists.emplace_back(&drawCommandList.get());
            }
        }
        device.executeCommandLists(m_commandLists.data(), m_commandLists.size(), nvrhi::CommandQueue::Graphics);
        m_commandLists.clear();
    }
}
