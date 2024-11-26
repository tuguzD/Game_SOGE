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
        m_inputCoreSDL.reset(aInputCore);
    }

    void KeyboardSDL::Update()
    {
        for (auto it : m_inputCoreSDL->m_sdlEventList)
        {
            SDL_Event* sdlEvent = &it;

            switch (sdlEvent->type)
            {
                case SDL_EVENT_KEY_DOWN:
                {
                    auto eventManager = Engine::GetInstance()->GetEventManager();
                    SOGE_INFO_LOG("Put KeyPressedEvent HERE");
                    break;
                }

                case SDL_EVENT_KEY_UP:
                {
                    SOGE_INFO_LOG("Some key up");
                    break;
                }

            }
        }
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
