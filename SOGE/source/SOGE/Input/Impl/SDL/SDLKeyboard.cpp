#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/SDLKeyboard.hpp"
#include "SOGE/Input/Impl/SDL/SDLInputCore.hpp"

#include "SOGE/Core/EventManager.hpp"
#include "SOGE/Event/InputEvents.hpp"
#include "SOGE/Event/CoreEvents.hpp"
#include "SOGE/Input/InputTypes.hpp"
#include "SOGE/Input/KeyMapManager.hpp"
#include "SOGE/Core/Engine.hpp"

#include <SDL3/SDL.h>


namespace soge
{
    SDLKeyboard::SDLKeyboard(SharedPtr<SDLInputCore> aInputCore)
        : Keyboard("SDL Portable Keyboard"), m_inputCoreSDL(aInputCore), m_repeatCounter(0)
    {
    }

    void SDLKeyboard::Update()
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
                SGScanCode sdlKeyCode = static_cast<SGScanCode>(sdlEvent->key.key);
                const Key& sogeKey = KeyMapManager::GetInstance()->GetKeyFromScanCode(sdlKeyCode);

                if (sdlEvent->key.repeat)
                    m_repeatCounter++;
                else
                    m_repeatCounter = 0;

                eventManager->Enqueue<KeyPressedEvent>(sogeKey, m_repeatCounter);
                break;
            }
            case SDL_EVENT_KEY_UP: {
                SGScanCode sdlKeyCode = static_cast<SGScanCode>(sdlEvent->key.key);
                const Key& sogeKey = KeyMapManager::GetInstance()->GetKeyFromScanCode(sdlKeyCode);
                eventManager->Enqueue<KeyReleasedEvent>(sogeKey);
                break;
            }
            default:
                break;
            }
        }

        eventManager->DispatchQueue<KeyPressedEvent>();
        eventManager->DispatchQueue<KeyReleasedEvent>();
    }

    bool SDLKeyboard::IsKeyPressed(Key aKeyName)
    {
        return false;
    }

    bool SDLKeyboard::IsKeyReleased(Key aKeyName)
    {
        return false;
    }

    Key SDLKeyboard::GetPressedKey()
    {
        return Key("");
    }

    Key SDLKeyboard::GetReleasedKey()
    {
        return Key("");
    }

    bool SDLKeyboard::IsAnyKeyPressed()
    {
        return false;
    }

    bool SDLKeyboard::IsAnyKeyReleased()
    {
        return false;
    }
}
