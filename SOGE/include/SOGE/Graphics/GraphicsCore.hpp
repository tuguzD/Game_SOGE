#ifndef SOGE_GRAPHICS_GRAPHICSCORE_HPP
#define SOGE_GRAPHICS_GRAPHICSCORE_HPP

#include "SOGE/DI/Dependency.hpp"

#include <nvrhi/nvrhi.h>


namespace soge
{
    class Window;
    class GraphicsSwapchain;

    class GraphicsCore
    {
    public:
        constexpr explicit GraphicsCore() noexcept = default;

        constexpr GraphicsCore(const GraphicsCore&) noexcept = delete;
        constexpr GraphicsCore& operator=(const GraphicsCore&) noexcept = delete;

        constexpr GraphicsCore(GraphicsCore&&) noexcept = default;
        constexpr GraphicsCore& operator=(GraphicsCore&&) noexcept = default;

        constexpr virtual ~GraphicsCore() noexcept = default;

        constexpr virtual void SetRenderTarget(const Window& aWindow) = 0;
        [[nodiscard]]
        constexpr virtual GraphicsSwapchain* GetSwapchain() = 0;

        constexpr virtual void Update(float aDeltaTime) = 0;

        [[nodiscard]]
        constexpr virtual nvrhi::IDevice& GetRawDevice() = 0;
        [[nodiscard]]
        constexpr virtual eastl::string_view GetCompiledShaderExtension() const = 0;
    };
}

SOGE_DI_REGISTER_NS(soge, GraphicsCore, df::Abstract<GraphicsCore>)

#endif // SOGE_GRAPHICS_GRAPHICSCORE_HPP
