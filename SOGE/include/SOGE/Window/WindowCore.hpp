#ifndef SOGE_WINDOW_WINDOWCORE_HPP
#define SOGE_WINDOW_WINDOWCORE_HPP

#include "SOGE/DI/Dependency.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Window/Window.hpp"

#pragma push_macro("CreateWindow")
#undef CreateWindow


namespace soge
{
    class WindowCore
    {
    public:
        constexpr explicit WindowCore() noexcept = default;

        constexpr explicit WindowCore(const WindowCore&) noexcept = delete;
        constexpr WindowCore& operator=(const WindowCore&) noexcept = delete;

        constexpr explicit WindowCore(WindowCore&&) noexcept = default;
        constexpr WindowCore& operator=(WindowCore&&) noexcept = default;

        constexpr virtual ~WindowCore() noexcept = default;

        virtual UniquePtr<Window> CreateWindow(const WindowDesc& aWindowSettings) = 0;
    };
}

SOGE_DI_REGISTER_NS(soge, WindowCore, df::Abstract<WindowCore>)


#pragma pop_macro("CreateWindow")

#endif // SOGE_WINDOW_WINDOWCORE_HPP
