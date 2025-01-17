#ifndef SOGE_GRAPHICS_GRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_GRAPHICSPIPELINE_HPP

#include <nvrhi/nvrhi.h>


namespace soge
{
    class GraphicsPipelineBase
    {
    public:
        constexpr explicit GraphicsPipelineBase() = default;

        constexpr GraphicsPipelineBase(const GraphicsPipelineBase&) = delete;
        constexpr GraphicsPipelineBase& operator=(const GraphicsPipelineBase&) = delete;

        constexpr GraphicsPipelineBase(GraphicsPipelineBase&&) noexcept = default;
        constexpr GraphicsPipelineBase& operator=(GraphicsPipelineBase&&) noexcept = default;

        constexpr virtual ~GraphicsPipelineBase() = default;

        [[nodiscard]]
        constexpr virtual nvrhi::IGraphicsPipeline& GetGraphicsPipeline() = 0;
    };

    template <typename E>
    class GraphicsPipeline : public GraphicsPipelineBase
    {
    public:
        using Entity = E;

        constexpr virtual void Execute(const nvrhi::Viewport& aViewport, const Camera& aCamera, Entity& aEntity,
                                       nvrhi::ICommandList& aCommandList) = 0;
    };
}

#endif // SOGE_GRAPHICS_GRAPHICSPIPELINE_HPP
