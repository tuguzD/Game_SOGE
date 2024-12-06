#ifndef SOGE_INPUT_IMPL_SDL_SDLKEYMAPMANAGER_HPP
#define SOGE_INPUT_IMPL_SDL_SDLKEYMAPMANAGER_HPP

#include "SOGE/Input/KeyMapManager.hpp"


namespace soge
{
    namespace impl
    {
        class SDLKeyMapManager final : public KeyMapManagerImpl
        {
        public:
            SDLKeyMapManager() = default;
            ~SDLKeyMapManager() = default;

            void SetupKeyMappings() override;
        };
    }

    typedef impl::SDLKeyMapManager ImplKeyMapManager;
}

#endif // !SOGE_INPUT_IMPL_SDL_SDLKEYMAPMANAGER_HPP
