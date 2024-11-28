#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/InputCoreSDL.hpp"
#include "SOGE/Input/Impl/SDL/KeyboardSDL.hpp"

#include <SDL3/SDL_init.h>

namespace soge
{
    InputCoreSDL::InputCoreSDL()
    {
        m_isPauseUpdateRequested = false;
        m_isEndUpdateRequested = false;
    }

    InputCoreSDL::~InputCoreSDL()
    {

    }

    void InputCoreSDL::LockInput(bool aLockInput)
    {

    }

    void InputCoreSDL::UseRelativeMouseMode(bool aRelMouse)
    {

    }

    void InputCoreSDL::BeginUpdateInput()
    {
        if (m_isPauseUpdateRequested)
            return;

        SDL_Event sdlEvent;
        m_sdlEventList.clear();
        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            m_sdlEventList.push_back(sdlEvent);
        }
    }

    void InputCoreSDL::EndUpdateInput()
    {
        m_isEndUpdateRequested = true;
    }

    void InputCoreSDL::SetPauseUpdate(bool aIsPauseNeeded)
    {
        m_isPauseUpdateRequested = aIsPauseNeeded;
    }

    Mouse* InputCoreSDL::CreateMouse()
    {
        return nullptr;
    }

    Gamepad* InputCoreSDL::CreateGamepad()
    {
        return nullptr;
    }

    Keyboard* InputCoreSDL::CreateKeyboard()
    {
        return new KeyboardSDL(this);
    }
}
