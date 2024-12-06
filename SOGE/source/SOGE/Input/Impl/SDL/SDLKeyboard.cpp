#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/SDLKeyboard.hpp"
#include "SOGE/Input/Impl/SDL/SDLInputCore.hpp"

#include "SOGE/Containers/FriendFuncPtr.hpp"
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

        eastl::list<SDL_Event>::iterator it = m_inputCoreSDL->m_sdlEventList.begin();
        for (; it != m_inputCoreSDL->m_sdlEventList.end(); ++it)
        {

            SDL_Event* sdlEvent = &(*it);

            switch (sdlEvent->type)
            {

            case SDL_EVENT_KEY_DOWN: {
                m_inputCoreSDL->m_isAnyButtonPressed = true;
                SGScanCode sdlKeyCode = static_cast<SGScanCode>(sdlEvent->key.key);
                const Key& sogeKey = KeyMapManager::GetInstance()->GetKeyFromScanCode(sdlKeyCode);
                KeyDetails* keyDetails = sogeKey.GetDetails();

                FriendFuncAccessor<KeyDetails, void, KeyState> accessor(KeyDetails::FriendlySetKeyState());
                accessor.Call(*keyDetails, KeyState_KeyPressed);
                if (sdlEvent->key.repeat)
                    m_repeatCounter++;
                else
                    m_repeatCounter = 0;

                eventManager->Enqueue<KeyPressedEvent>(sogeKey, m_repeatCounter);
                break;
            }

            case SDL_EVENT_KEY_UP: {
                m_inputCoreSDL->m_isAnyButtonPressed = false;
                SGScanCode sdlKeyCode = static_cast<SGScanCode>(sdlEvent->key.key);
                const Key& sogeKey = KeyMapManager::GetInstance()->GetKeyFromScanCode(sdlKeyCode);
                KeyDetails* keyDetails = sogeKey.GetDetails();

                FriendFuncAccessor<KeyDetails, void, KeyState> accessor(KeyDetails::FriendlySetKeyState());
                accessor.Call(*keyDetails, KeyState_KeyReleased);
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

    bool SDLKeyboard::IsKeyPressed(const Key aKeyName)
    {
        if (aKeyName.GetKeyState() == KeyState_KeyPressed)
            return true;
        return false;
    }

    bool SDLKeyboard::IsKeyReleased(const Key aKeyName)
    {
        if (aKeyName.GetKeyState() != KeyState_KeyPressed)
            return true;
        return false;
    }
}
