#include "sogepch.hpp"

#include "SOGE/Graphics/SimpleRenderGraph.hpp"

#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"
#include "SOGE/Graphics/TriangleGraphicsPipeline.hpp"


namespace soge
{
    SimpleRenderGraph::SimpleRenderGraph(GraphicsCore& aCore) : RenderGraph{aCore}, m_core{aCore}
    {
        m_renderPass = CreateUnique<FinalGraphicsRenderPass>(aCore);
        m_pipeline = CreateUnique<TriangleGraphicsPipeline>(aCore, *m_renderPass);
    }

    SimpleRenderGraph::~SimpleRenderGraph() = default;

    void SimpleRenderGraph::Execute(const float aDeltaTime)
    {
        nvrhi::IDevice& device = m_core.get().GetRawDevice();

        const auto clearCommandList = m_renderPass->CreateClearCommandList();
        const auto drawCommandLists = m_pipeline->Update(aDeltaTime);

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
