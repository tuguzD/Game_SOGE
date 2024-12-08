#include "sogepch.hpp"

#include "SOGE/System/Impl/SDL/SDLContext.hpp"

#include <SDL3/SDL_init.h>


namespace soge
{
    SDLContext::SDLContext()
    {
        SOGE_INFO_LOG("Initializing SDL...");

        if (SDL_Init(SDL_INIT_EVENTS))
        {
            SOGE_INFO_LOG("SDL initialization was successful!");
        }
        else
        {
            SOGE_ERROR_LOG("SDL initialization failed: {}", SDL_GetError());
        }
    }

    SDLContext::~SDLContext()
    {
        SOGE_INFO_LOG("Cleaning up SDL...");

        SDL_Quit();
    }
}
