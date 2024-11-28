#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/KeyboardSDL.hpp"
#include "SOGE/Input/Impl/SDL/InputCoreSDL.hpp"

#include "SOGE/Core/EventManager.hpp"
#include "SOGE/Event/InputEvents.hpp"
#include "SOGE/Event/CoreEvents.hpp"
#include "SOGE/Input/InputTypes.hpp"
#include "SOGE/Core/Engine.hpp"

#include <SDL3/SDL.h>


namespace soge
{
    KeyboardSDL::KeyboardSDL(InputCoreSDL* aInputCore) : Keyboard("SDL Portable Keyboard")
    {
        m_inputCoreSDL = SharedPtr<InputCoreSDL>(aInputCore);
    }

    void KeyboardSDL::Update()
    {
        auto eventManager = Engine::GetInstance()->GetEventManager();
        if (!eventManager)
        {
            return;
        }

        for (auto it : m_inputCoreSDL->m_sdlEventList)
        {

            SDL_Event* sdlEvent = &it;

            switch (sdlEvent->type)
            {
            case SDL_EVENT_KEY_DOWN: {
                // TODO convert from SDL key to our key
                eventManager->Enqueue<KeyPressedEvent>(Keys::A, 1);
                break;
            }
            case SDL_EVENT_KEY_UP: {
                // TODO convert from SDL key to our key
                eventManager->Enqueue<KeyReleasedEvent>(Keys::A);
                break;
            }
            default:
                break;
            }
        }

        eventManager->DispatchQueue<KeyPressedEvent>();
        eventManager->DispatchQueue<KeyReleasedEvent>();
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
