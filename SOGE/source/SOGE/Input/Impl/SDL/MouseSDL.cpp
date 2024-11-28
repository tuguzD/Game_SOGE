#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/MouseSDL.hpp"


namespace soge
{
    MouseSDL::MouseSDL(SharedPtr<InputCoreSDL> aInputCore) : Mouse("SDL Portable Mouse")
    {
    }

    void MouseSDL::Update()
    {
    }

    bool MouseSDL::IsButtonPressed()
    {
        return false;
    }

    bool MouseSDL::IsButtonReleased()
    {
        return false;
    }
}
