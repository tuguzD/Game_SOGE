#ifndef SOGE_GRAPHICS_GRAPHICSRENDERPASS_HPP
#define SOGE_GRAPHICS_GRAPHICSRENDERPASS_HPP

#include <EASTL/functional.h>
#include <nvrhi/nvrhi.h>


namespace soge
{
    class GraphicsRenderPass
    {
    public:
        constexpr explicit GraphicsRenderPass() = default;

        constexpr GraphicsRenderPass(const GraphicsRenderPass&) = delete;
        constexpr GraphicsRenderPass& operator=(const GraphicsRenderPass&) = delete;

        constexpr GraphicsRenderPass(GraphicsRenderPass&&) noexcept = default;
        constexpr GraphicsRenderPass& operator=(GraphicsRenderPass&&) noexcept = default;

        constexpr virtual ~GraphicsRenderPass() = default;

        using FramebufferRef = eastl::reference_wrapper<nvrhi::IFramebuffer>;

        [[nodiscard]]
        constexpr virtual FramebufferRef GetFramebuffer() = 0;
    };
}

#endif // SOGE_GRAPHICS_GRAPHICSRENDERPASS_HPP
