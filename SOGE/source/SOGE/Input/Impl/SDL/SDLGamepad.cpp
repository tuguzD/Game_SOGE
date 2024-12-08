#include "sogepch.hpp"

#include "SOGE/Input/Impl/SDL/SDLGamepad.hpp"


namespace soge
{
    SDLGamepad::SDLGamepad(SDLInputCore& aInputCore) : Gamepad("SDL Portable Gamepad"), m_inputCoreSDL(aInputCore)
    {
    }
}
