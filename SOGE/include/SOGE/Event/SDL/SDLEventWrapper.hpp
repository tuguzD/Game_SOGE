#ifndef SOGE_EVENT_SDL_SDLEVENTWRAPPER_HPP
#define SOGE_EVENT_SDL_SDLEVENTWRAPPER_HPP

#include <SDL3/SDL_events.h>


namespace soge
{
    class SDLEventWrapper
    {
    private:
        SDL_Event* m_sdlEvent;

    public:
        SDLEventWrapper() = default;
        SDLEventWrapper(SDL_Event* aSDLEvent);
        ~SDLEventWrapper();

        SDL_Event* Get();

    };
}

#endif // !SOGE_EVENT_SDL_SDLEVENTWRAPPER_HPP
