#ifndef SOGE_SYSTEM_IMPL_SDL_SDLWINDOW_HPP
#define SOGE_SYSTEM_IMPL_SDL_SDLWINDOW_HPP

#include "SOGE/System/Window.hpp"
#include "SOGE/System/Memory.hpp"

#include <SDL3/SDL_video.h>


namespace soge
{
    class SDLWindow final : public Window
    {
    private:
        SDL_Window* m_sdlWindow;
        WindowDesc m_windowDescriptor;

    public:
        SDLWindow(WindowDesc& aWindowDescriptor);
        ~SDLWindow();

        void* GetNativeHandler() const override;
        eastl::wstring GetTitle() const override;
        std::uint32_t GetWidth() const override;
        std::uint32_t GetHeight() const override;
    };
}

#endif // SOGE_SYSTEM_IMPL_SDL_SDLWINDOW_HPP
