#ifndef SOGE_INPUT_IMPL_SDL_SDLGAMEPAD_HPP
#define SOGE_INPUT_IMPL_SDL_SDLGAMEPAD_HPP

#include "SOGE/Input/Device/Gamepad.hpp"
#include "SOGE/Input/Impl/SDL/SDLInputCore.hpp"


namespace soge
{
    class SDLGamepad final : public Gamepad
    {
    private:
        SDLInputCore* m_inputCoreSDL;

    public:
        explicit SDLGamepad(SDLInputCore& aInputCore);
    };

    using ImplGamepad = SDLGamepad;
}

#endif // SOGE_INPUT_IMPL_SDL_SDLGAMEPAD_HPP
