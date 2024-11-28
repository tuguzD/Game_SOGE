#include "sogepch.hpp"
#include "SOGE/Event/SDL/SDLEventWrapper.hpp"


namespace soge
{
    SDLEventWrapper::SDLEventWrapper(SDL_Event* aSDLEvent) : m_sdlEvent(aSDLEvent)
    {
    }

    SDLEventWrapper::~SDLEventWrapper()
    {
        delete m_sdlEvent;
    }

    SDL_Event* SDLEventWrapper::Get()
    {
        return m_sdlEvent;
    }
}
