#include "sogepch.hpp"

#include "SOGE/Graphics/Deferred/DeferredRenderGraph.hpp"

#include "SOGE/Graphics/GraphicsEntity.hpp"
#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"


namespace soge
{
    DeferredRenderGraph::DeferredRenderGraph(GraphicsCore& aCore, GeometryGraphicsRenderPass& aGeometryPass,
                                             FinalGraphicsRenderPass& aFinalPass,
                                             GeometryGraphicsPipeline& aGeometryPipeline,
                                             AmbientLightGraphicsPipeline& aAmbientLightPipeline)
        : RenderGraph{aCore}, m_core{aCore}, m_geometryPass{aGeometryPass}, m_finalPass{aFinalPass},
          m_geometryPipeline{aGeometryPipeline}, m_ambientLightPipeline{aAmbientLightPipeline}
    {
    }

    void DeferredRenderGraph::Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, const Entities aEntities)
    {
        GraphicsCore& core = m_core;
        nvrhi::IDevice& device = core.GetRawDevice();

        nvrhi::CommandListParameters commandListDesc{};
        commandListDesc.enableImmediateExecution = false;

        const nvrhi::CommandListHandle commandList = device.createCommandList(commandListDesc);
        {
            GraphicsCommandListGuard commandListGuard{*commandList};

            m_geometryPass.get().ClearFramebuffer(commandListGuard);
            for (auto&& entityRef : aEntities)
            {
                if (const auto entity = dynamic_cast<GeometryGraphicsPipeline::Entity*>(&entityRef.get()))
                {
                    m_geometryPipeline.get().Execute(aViewport, aCamera, *entity, commandListGuard);
                }
            }

            m_finalPass.get().ClearFramebuffer(commandListGuard);
            {
                const auto destDepthTexture = m_finalPass.get().GetFramebuffer().getDesc().depthAttachment.texture;
                const auto srcDepthTexture = m_geometryPass.get().GetFramebuffer().getDesc().depthAttachment.texture;
                commandListGuard->copyTexture(destDepthTexture, {}, srcDepthTexture, {});
            }

            for (auto&& entityRef : aEntities)
            {
                if (const auto entity = dynamic_cast<AmbientLightGraphicsPipeline::Entity*>(&entityRef.get()))
                {
                    m_ambientLightPipeline.get().Execute(aViewport, aCamera, *entity, commandListGuard);
                }
            }
        }
        core.ExecuteCommandList(commandList, nvrhi::CommandQueue::Graphics);
    }
}
