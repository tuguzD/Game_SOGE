#ifndef SOGE_SYSTEM_IMPL_SDL_SDLCONTEXT_HPP
#define SOGE_SYSTEM_IMPL_SDL_SDLCONTEXT_HPP

#include "SOGE/DI/Dependency.hpp"


namespace soge
{
    class SDLContext final
    {
    public:
        explicit SDLContext();

        explicit SDLContext(const SDLContext&) = delete;
        SDLContext& operator=(const SDLContext&) = delete;

        explicit SDLContext(SDLContext&&) noexcept = default;
        SDLContext& operator=(SDLContext&&) noexcept = default;

        ~SDLContext();
    };
}

SOGE_DI_REGISTER_NS(soge, SDLContext, df::Single<SDLContext>, tag::Final<SDLContext>)

#endif // SOGE_SYSTEM_IMPL_SDL_SDLCONTEXT_HPP
