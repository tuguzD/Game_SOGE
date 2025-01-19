#ifndef SOGE_INPUT_IMPL_SDL_SDLKEYMAPMANAGER_HPP
#define SOGE_INPUT_IMPL_SDL_SDLKEYMAPMANAGER_HPP

#include "SOGE/Input/KeyMapManager.hpp"


namespace soge
{
    class SDLKeyMapManager final : public KeyMapManager
    {
    public:
        explicit SDLKeyMapManager();
    };

    using ImplKeyMapManager = SDLKeyMapManager;
}

#endif // SOGE_INPUT_IMPL_SDL_SDLKEYMAPMANAGER_HPP
