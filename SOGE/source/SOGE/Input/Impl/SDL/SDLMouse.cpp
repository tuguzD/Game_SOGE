#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/SDLMouse.hpp"


namespace soge
{
    SDLMouse::SDLMouse(SharedPtr<SDLInputCore> aInputCore) : Mouse("SDL Portable Mouse")
    {
    }

    void SDLMouse::Update()
    {
    }

    bool SDLMouse::IsButtonPressed()
    {
        return false;
    }

    bool SDLMouse::IsButtonReleased()
    {
        return false;
    }
}
