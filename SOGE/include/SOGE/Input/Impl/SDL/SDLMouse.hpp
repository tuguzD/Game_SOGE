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
        eastl::pair<float*, float*> m_coords;
        int m_repeatCounter;


    public:
        SDLMouse(SharedPtr<SDLInputCore> aInputCore);
        ~SDLMouse() = default;

        void Update() override;
        bool IsButtonPressed(const Key aMouseButton) override;
        bool IsButtonReleased(const Key aMouseButton) override;

    };

    typedef SDLMouse ImplMouse;

}

#endif // SOGE_INPUT_IMPL_SDL_SDLMOUSE_HPP
