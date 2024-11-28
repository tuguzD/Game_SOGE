#ifndef SOGE_INPUT_IMPL_GAMEPADSDL_HPP
#define SOGE_INPUT_IMPL_GAMEPADSDL_HPP

#include "SOGE/Input/Impl/SDL/InputCoreSDL.hpp"
#include "SOGE/Input/InputTypes.hpp"
#include "SOGE/Input/Gamepad.hpp"


namespace soge
{
    class GamepadSDL final : public Gamepad
    {
    private:
        SharedPtr<InputCoreSDL> m_inputCoreSDL;

    public:
        GamepadSDL(SharedPtr<InputCoreSDL> aInputCore);
        ~GamepadSDL() = default;

    };
}

#endif // !SOGE_INPUT_IMPL_GAMEPADSDL_HPP
