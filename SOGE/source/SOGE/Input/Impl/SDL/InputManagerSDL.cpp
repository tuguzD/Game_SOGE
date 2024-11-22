#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/InputManagerSDL.hpp"
#include "SOGE/Input/Impl/SDL/KeyboardSDL.hpp"

#include <SDL3/SDL.h>


namespace soge
{
    InputManagerSDL::InputManagerSDL()
    {
        LockInput(true);
        UseRelativeMouseMode(false);

        SDL_InitSubSystem(SDL_INIT_GAMEPAD);
    }

    InputManagerSDL::~InputManagerSDL()
    {
    }

    void InputManagerSDL::LockInput(bool aLockInput)
    {
    }

    void InputManagerSDL::UseRelativeMouseMode(bool aRelMouse)
    {
    }

    void InputManagerSDL::BeginUpdateInput()
    {
        SDL_Event sdlEvent;

        m_eventList.clear();
        while (SDL_PollEvent(&sdlEvent) != 0)
        {

        }
    }

    void InputManagerSDL::EndUpdateInput()
    {
    }

}
