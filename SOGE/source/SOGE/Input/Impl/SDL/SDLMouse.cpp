#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/SDLMouse.hpp"

#include "SOGE/Containers/FriendFuncPtr.hpp"
#include "SOGE/Event/InputEvents.hpp"
#include "SOGE/Input/InputTypes.hpp"
#include "SOGE/Input/KeyMapManager.hpp"
#include "SOGE/Core/Engine.hpp"

#include <SDL3/SDL.h>


namespace soge
{
    SDLMouse::SDLMouse(SharedPtr<SDLInputCore> aInputCore)
        : Mouse("SDL Portable Mouse"), m_inputCoreSDL(aInputCore), m_repeatCounter(0), m_coords(0, 0)
    {
    }

    void SDLMouse::Update()
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
            case SDL_EVENT_MOUSE_MOTION: {
                float x, y;
                SDL_GetMouseState(&x, &y);
                eventManager->Enqueue<MouseMovedEvent>(x, y);

                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                SGScanCode sdlButtonCode = static_cast<SGScanCode>(sdlEvent->button.button);
                const Key& sogeButton = KeyMapManager::GetInstance()->GetKeyFromScanCode(sdlButtonCode);
                KeyDetails* buttonDetails = sogeButton.GetDetails();

                FriendFuncAccessor<KeyDetails, KeyDetails, void, KeyState> accessor(KeyDetails::FriendlySetKeyState());
                accessor.Call(*buttonDetails, KeyState_KeyPressed);
                if (sdlEvent->key.repeat)
                    m_repeatCounter++;
                else
                    m_repeatCounter = 0;

                eventManager->Enqueue<MouseButtonPressedEvent>(sogeButton, m_repeatCounter);
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_UP: {
                SGScanCode sdlButtonCode = static_cast<SGScanCode>(sdlEvent->button.button);
                const Key& sogeButton = KeyMapManager::GetInstance()->GetKeyFromScanCode(sdlButtonCode);
                KeyDetails* buttonDetails = sogeButton.GetDetails();

                FriendFuncAccessor<KeyDetails, KeyDetails, void, KeyState> accessor(KeyDetails::FriendlySetKeyState());
                accessor.Call(*buttonDetails, KeyState_KeyReleased);

                eventManager->Enqueue<MouseButtonReleasedEvent>(sogeButton);
                break;
            }

            case SDL_EVENT_MOUSE_WHEEL: {
                float xOffset = sdlEvent->motion.x;
                float yOffset = sdlEvent->motion.y;
                eventManager->Enqueue<MouseWheelEvent>(xOffset, yOffset);

                break;
            }

            default:
                break;
            }

            eventManager->DispatchQueue<MouseMovedEvent>();
            eventManager->DispatchQueue<MouseButtonPressedEvent>();
            eventManager->DispatchQueue<MouseButtonReleasedEvent>();
            eventManager->DispatchQueue<MouseWheelEvent>();

        }
    }

    bool SDLMouse::IsButtonPressed(const Key aMouseButton)
    {
        if (aMouseButton.GetKeyState() == KeyState_KeyPressed)
            return true;
        return false;
    }

    bool SDLMouse::IsButtonReleased(const Key aMouseButton)
    {
        if (aMouseButton.GetKeyState() != KeyState_KeyPressed)
            return true;
        return false;
    }
}
