#ifndef SOGE_INPUT_IMPL_SDL_MOUSE_HPP
#define SOGE_INPUT_IMPL_SDL_MOUSE_HPP

#include "SOGE/Input/Impl/SDL/InputCoreSDL.hpp"
#include "SOGE/Input/InputTypes.hpp"
#include "SOGE/Input/Mouse.hpp"


namespace soge
{
    class MouseSDL final : public Mouse
    {
    private:
        SharedPtr<InputCoreSDL> m_inputCoreSDL;

    public:
        MouseSDL(SharedPtr<InputCoreSDL> aInputCore);
        ~MouseSDL() = default;

        void Update() override;

        bool IsButtonPressed() override;
        bool IsButtonReleased() override;

    };
}

#endif // !SOGE_INPUT_IMPL_SDL_MOUSE_HPP
