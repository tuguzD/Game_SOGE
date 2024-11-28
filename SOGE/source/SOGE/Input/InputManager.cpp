#include "sogepch.hpp"
#include "SOGE/Input/InputManager.hpp"

#include "SOGE/Input/Impl/SDL/InputCoreSDL.hpp"
#include "SOGE/Input/Impl/SDL/KeyboardSDL.hpp"
#include "SOGE/Input/Impl/SDL/MouseSDL.hpp"

#include <SDL3/SDL.h>


namespace soge
{
    InputManager::InputManager()
    {
        if (!SDL_Init(SDL_INIT_EVENTS))
        {
            SOGE_ERROR_LOG("Failed to initialize SDL events subsystem...");
        }

        m_inputCore.reset(new InputCoreSDL());
        m_keyboard.reset(m_inputCore->CreateKeyboard());
        m_mouse.reset(m_inputCore->CreateMouse());
    }

    void InputManager::Update()
    {
        m_inputCore->BeginUpdateInput();
        m_keyboard->Update();
    }
}
