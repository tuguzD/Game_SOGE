#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/GamepadSDL.hpp"


namespace soge
{
    GamepadSDL::GamepadSDL(SharedPtr<InputCoreSDL> aInputCore) : Gamepad("SDL Portable Gamepad")
    {
        m_inputCoreSDL = aInputCore;
    }
}
