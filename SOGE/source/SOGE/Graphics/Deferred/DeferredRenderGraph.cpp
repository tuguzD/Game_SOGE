#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/DeferredRenderGraph.hpp"

#include "SOGE/Graphics/GraphicsEntity.hpp"
#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"


namespace soge
{
    DeferredRenderGraph::DeferredRenderGraph(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryPass,
                                             FinalGraphicsRenderPass& aFinalPass,
                                             GeometryGraphicsPipeline& aGeometryPipeline,
                                             LightGraphicsPipeline& aLightPipeline)
        : RenderGraph{aCore}, m_core{aCore}, m_geometryPass{aGeometryPass}, m_finalPass{aFinalPass},
          m_geometryPipeline{aGeometryPipeline}, m_lightPipeline{aLightPipeline}
    {
    }

    void DeferredRenderGraph::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, const Entities aEntities)
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

        const nvrhi::CommandListHandle lightCommandList = device.createCommandList(commandListDesc);
        {
            GraphicsCommandListGuard commandList{*lightCommandList};

            m_finalPass.get().ClearFramebuffer(commandList);
            {
                const auto destDepthTexture = m_finalPass.get().GetFramebuffer().getDesc().depthAttachment.texture;
                const auto srcDepthTexture = m_geometryPass.get().GetFramebuffer().getDesc().depthAttachment.texture;
                commandList->copyTexture(destDepthTexture, {}, srcDepthTexture, {});
            }

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
