  #ifndef SOGE_INPUT_IMPL_SDL_SDLMOUSE_HPP
#define SOGE_INPUT_IMPL_SDL_SDLMOUSE_HPP

#include "SOGE/Input/Impl/SDL/SDLInputCore.hpp"
#include "SOGE/Input/Device/Mouse.hpp"
#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    class SDLMouse final : public Mouse
    {
    private:
        SharedPtr<SDLInputCore> m_inputCoreSDL;

    public:
        SDLMouse(SharedPtr<SDLInputCore> aInputCore);
        ~SDLMouse() = default;

        void Update() override;

        bool IsButtonPressed() override;
        bool IsButtonReleased() override;

    };

    typedef SDLMouse ImplMouse;

}

#endif // SOGE_INPUT_IMPL_SDL_SDLMOUSE_HPP
