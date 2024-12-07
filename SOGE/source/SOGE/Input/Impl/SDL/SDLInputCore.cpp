#include "sogepch.hpp"

#include "SOGE/Input/Impl/SDL/SDLInputCore.hpp"

#include "SOGE/Input/Impl/SDL/SDLGamepad.hpp"
#include "SOGE/Input/Impl/SDL/SDLKeyMapManager.hpp"
#include "SOGE/Input/Impl/SDL/SDLKeyboard.hpp"
#include "SOGE/Input/Impl/SDL/SDLMouse.hpp"

#include <SDL3/SDL_init.h>


namespace soge
{
    SDLInputCore::SDLInputCore(EventModule* aEventModule)
        : InputCore(aEventModule), m_isPauseUpdateRequested(false), m_isEndUpdateRequested(false),
          m_isAnyButtonPressed(false)
    {
        if (!SDL_Init(SDL_INIT_EVENTS))
        {
            SOGE_ERROR_LOG("Failed to initialize SDL event subsystem...");
            return;
        }

        m_keyMapManager = CreateUnique<SDLKeyMapManager>();
        m_keyboard = CreateUnique<SDLKeyboard>(*this);
        m_gamepad = CreateUnique<SDLGamepad>(*this);
        m_mouse = CreateUnique<SDLMouse>(*this);
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

        m_mouse->Update();
        m_keyboard->Update();
    }

    void SDLInputCore::EndUpdateInput()
    {
        m_isEndUpdateRequested = true;
    }

    void SDLInputCore::SetPauseUpdate(const bool aIsPauseNeeded)
    {
        m_isPauseUpdateRequested = aIsPauseNeeded;
    }
}
