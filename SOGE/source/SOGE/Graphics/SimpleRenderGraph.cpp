#include "sogepch.hpp"

#include "SOGE/Graphics/SimpleRenderGraph.hpp"

#include "SOGE/Graphics/GraphicsEntity.hpp"
#include "SOGE/Graphics/TriangleGraphicsPipeline.hpp"


namespace soge
{
    SimpleRenderGraph::SimpleRenderGraph(GraphicsCore& aCore, FinalGraphicsRenderPass& aFinalRenderPass,
                                         TriangleGraphicsPipeline& aTrianglePipeline)
        : RenderGraph{aCore}, m_core{aCore}, m_finalRenderPass{aFinalRenderPass}, m_trianglePipeline{aTrianglePipeline}
    {
    }

    void SimpleRenderGraph::Execute(const Entities aEntities)
    {
        const auto clearCommandList = m_finalRenderPass.get().CreateClearFramebufferCommandList();

        m_commandLists.reserve(aEntities.size() + 1);
        m_commandLists.emplace_back(clearCommandList);
        for (const auto& entity : aEntities)
        {
            auto commandList = entity.get().Update(m_finalRenderPass, m_trianglePipeline);
            if (commandList == nullptr)
            {
                continue;
            }
            m_commandLists.emplace_back(commandList);
        }

        m_commandListPtrs.reserve(m_commandLists.size());
        for (const auto& commandList : m_commandLists)
        {
            m_commandListPtrs.emplace_back(commandList.Get());
        }

        nvrhi::IDevice& device = m_core.get().GetRawDevice();
        device.executeCommandLists(m_commandListPtrs.data(), m_commandListPtrs.size(), nvrhi::CommandQueue::Graphics);

        m_commandListPtrs.clear();
        m_commandLists.clear();
    }
}
