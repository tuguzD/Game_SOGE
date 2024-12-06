#ifndef SOGE_INPUT_IMPL_GAMEPADSDL_HPP
#define SOGE_INPUT_IMPL_GAMEPADSDL_HPP

#include "SOGE/Input/Impl/SDL/SDLInputCore.hpp"
#include "SOGE/Input/Device/Gamepad.hpp"
#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    class SDLGamepad final : public Gamepad
    {
    private:
        SharedPtr<SDLInputCore> m_inputCoreSDL;

    public:
        SDLGamepad(SharedPtr<SDLInputCore> aInputCore);
        ~SDLGamepad() = default;

    };

    typedef SDLGamepad ImplGamepad;

}

#endif // SOGE_INPUT_IMPL_GAMEPADSDL_HPP
