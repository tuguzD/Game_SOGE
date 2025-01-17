#include "sogepch.hpp"

#include "SOGE/Graphics/SimpleRenderGraph.hpp"

#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    SimpleRenderGraph::SimpleRenderGraph(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryPass,
                                         FinalGraphicsRenderPass& aFinalPass,
                                         TriangleGraphicsPipeline& aTrianglePipeline,
                                         GeometryGraphicsPipeline& aGeometryPipeline,
                                         LightGraphicsPipeline& aLightPipeline)
        : RenderGraph{aCore}, m_core{aCore}, m_geometryPass{aGeometryPass}, m_finalPass{aFinalPass},
          m_trianglePipeline{aTrianglePipeline}, m_geometryPipeline{aGeometryPipeline}, m_lightPipeline{aLightPipeline}
    {
    }

    void SimpleRenderGraph::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, const Entities aEntities)
    {
        GraphicsCore& core = m_core;
        nvrhi::IDevice& device = core.GetRawDevice();

        nvrhi::CommandListParameters commandListDesc{};
        commandListDesc.enableImmediateExecution = false;

        const nvrhi::CommandListHandle geometryCommandList = device.createCommandList(commandListDesc);
        {
            GraphicsCommandListGuard commandList{*geometryCommandList};

            m_geometryPass.get().ClearFramebuffer(commandList);

            for (auto&& entityRef : aEntities)
            {
                if (const auto entity = dynamic_cast<GeometryGraphicsPipeline::Entity*>(&entityRef.get()))
                {
                    m_geometryPipeline.get().Execute(aViewport, aCamera, *entity, commandList);
                }
            }
        }
        core.ExecuteCommandList(geometryCommandList, nvrhi::CommandQueue::Graphics);

        // const auto triangleCommandLists = m_trianglePipeline.get().Execute(aViewport, aCamera, aEntities);
        // m_core.get().ExecuteCommandLists(triangleCommandLists, nvrhi::CommandQueue::Graphics);

        const nvrhi::CommandListHandle lightCommandList = device.createCommandList(commandListDesc);
        {
            GraphicsCommandListGuard commandList{*lightCommandList};

            m_finalPass.get().ClearFramebuffer(commandList);

            LightGraphicsPipeline::Entity entity{};
            m_lightPipeline.get().Execute(aViewport, aCamera, entity, commandList);
            // for (auto&& entityRef : aEntities)
            // {
            //     if (const auto entity = dynamic_cast<LightGraphicsPipeline::Entity*>(&entityRef.get()))
            //     {
            //         m_lightPipeline.get().Execute(aViewport, aCamera, *entity, commandList);
            //     }
            // }
        }
        core.ExecuteCommandList(lightCommandList, nvrhi::CommandQueue::Graphics);
    }
}
