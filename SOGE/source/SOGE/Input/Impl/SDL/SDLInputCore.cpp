#include "sogepch.hpp"

#include "SOGE/Input/Impl/SDL/SDLInputCore.hpp"
#include "SOGE/Input/Impl/SDL/SDLKeyboard.hpp"
#include "SOGE/Input/Impl/SDL/SDLMouse.hpp"

#include <SDL3/SDL_init.h>


namespace soge
{
    SDLInputCore::SDLInputCore()
        : m_isPauseUpdateRequested(false), m_isEndUpdateRequested(false), m_isAnyButtonPressed(false)
    {
        if (!SDL_Init(SDL_INIT_EVENTS))
        {
            SOGE_ERROR_LOG("Failed to initialize SDL events subsystem...");
        }
    }

    void SDLInputCore::LockInput(bool aLockInput)
    {
    }

    void SDLInputCore::UseRelativeMouseMode(bool aRelMouse)
    {
    }

    bool SDLInputCore::IsAnyButtonPressed()
    {
        return m_isAnyButtonPressed;
    }

    void SDLInputCore::BeginUpdateInput()
    {
        if (m_isPauseUpdateRequested)
        {
            return;
        }

        SDL_Event sdlEvent;
        m_sdlEventList.clear();
        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            m_sdlEventList.push_back(sdlEvent);
        }
    }

    void SDLInputCore::EndUpdateInput()
    {
        m_isEndUpdateRequested = true;
    }

    void SDLInputCore::SetPauseUpdate(const bool aIsPauseNeeded)
    {
        m_isPauseUpdateRequested = aIsPauseNeeded;
    }

    Mouse* SDLInputCore::CreateMouse()
    {
        return new SDLMouse(shared_from_this());
    }

    Gamepad* SDLInputCore::CreateGamepad()
    {
        return nullptr;
    }

    Keyboard* SDLInputCore::CreateKeyboard()
    {
        return new SDLKeyboard(shared_from_this());
    }
}
