#include "sogepch.hpp"
#include "SOGE/System/Impl/SDL/SDLWindow.hpp"
#include "SOGE/Utils/StringHelpers.hpp"


namespace soge
{
    Window* Window::Create(WindowDesc aWindowDescriptor)
    {
        return new SDLWindow(aWindowDescriptor);
    }

    SDLWindow::SDLWindow(WindowDesc& aWindowDescriptor)
    {
        m_windowDescriptor = aWindowDescriptor;

        m_sdlWindow = SDL_CreateWindow(
            EAToNarrow(m_windowDescriptor.m_title).c_str(),
            m_windowDescriptor.m_width,
            m_windowDescriptor.m_height,
            SDL_WINDOW_INPUT_FOCUS
        );

        //SDL_SetWindowRelativeMouseMode(m_sdlWindow, true);
    }

    SDLWindow::~SDLWindow()
    {
        SDL_DestroyWindow(m_sdlWindow);
    }

    void* SDLWindow::GetNativeHandler() const
    {
        SDL_PropertiesID properties = SDL_GetWindowProperties(m_sdlWindow);
        return (void*)SDL_PROP_WINDOW_WIN32_HWND_POINTER;
    }

    eastl::wstring SDLWindow::GetTitle() const
    {
        return EAToWide(SDL_GetWindowTitle(m_sdlWindow));
    }

    std::uint32_t SDLWindow::GetWidth() const
    {
        return std::uint32_t();
    }

    std::uint32_t SDLWindow::GetHeight() const
    {
        return std::uint32_t();
    }
}
