#include "sogepch.hpp"

#include "SOGE/Window/Window.hpp"


namespace soge
{
    WindowDesc::WindowDesc(eastl::wstring aTitle, const std::uint32_t aWidth, const std::uint32_t aHeight)
        : m_title(std::move(aTitle)), m_width(aWidth), m_height(aHeight)
    {
    }

    Window::Window([[maybe_unused]] const WindowDesc& aWindowDesc)
    {
    }
}
