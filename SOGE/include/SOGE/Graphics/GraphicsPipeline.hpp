#ifndef SOGE_GRAPHICS_GRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_GRAPHICSPIPELINE_HPP

#include <EASTL/functional.h>
#include <EASTL/span.h>
#include <nvrhi/nvrhi.h>


namespace soge
{
    class GraphicsPipeline
    {
    public:
        constexpr explicit GraphicsPipeline() = default;

        constexpr explicit GraphicsPipeline(const GraphicsPipeline&) = delete;
        constexpr GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

        constexpr explicit GraphicsPipeline(GraphicsPipeline&&) noexcept = default;
        constexpr GraphicsPipeline& operator=(GraphicsPipeline&&) noexcept = default;

        constexpr virtual ~GraphicsPipeline() = default;

        using CommandListRef = eastl::reference_wrapper<nvrhi::ICommandList>;
        using CommandLists = eastl::span<CommandListRef>;
        constexpr virtual CommandLists Update(float aDeltaTime) = 0;
    };
}

#endif // SOGE_GRAPHICS_GRAPHICSPIPELINE_HPP
