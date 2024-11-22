#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/KeyboardSDL.hpp"

#include <SDL3/SDL.h>


namespace soge
{
    KeyboardSDL::KeyboardSDL() : Keyboard("SDL Portable Keyboard")
    {

    }

    bool KeyboardSDL::IsKeyPressed(Key aKeyName)
    {
        return false;
    }

    bool KeyboardSDL::IsKeyReleased(Key aKeyName)
    {
        return false;
    }

    Key KeyboardSDL::GetPressedKey()
    {
        return Key("");
    }

    Key KeyboardSDL::GetReleasedKey()
    {
        return Key("");
    }

    bool KeyboardSDL::IsAnyKeyPressed()
    {
        return false;
    }

    bool KeyboardSDL::IsAnyKeyReleased()
    {
        return false;
    }
}
