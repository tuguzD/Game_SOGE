#include "sogepch.hpp"

#include "SOGE/Input/Impl/SDL/SDLMouse.hpp"

#include "SOGE/Containers/FriendFuncPtr.hpp"
#include "SOGE/Core/Engine.hpp"
#include "SOGE/Event/InputEvents.hpp"
#include "SOGE/Input/InputTypes.hpp"
#include "SOGE/Input/KeyMapManager.hpp"

#include <SDL3/SDL.h>


namespace soge
{
    SDLMouse::SDLMouse(const SharedPtr<SDLInputCore>& aInputCore)
        : Mouse("SDL Portable Mouse"), m_inputCoreSDL(aInputCore), m_repeatCounter(0)
    {
    }

    void SDLMouse::Update()
    {
        const auto eventManager = Engine::GetInstance()->GetEventManager();
        if (!eventManager)
        {
            return;
        }

        for (const auto& sdlEvent : m_inputCoreSDL->m_sdlEventList)
        {
            switch (sdlEvent.type)
            {
            case SDL_EVENT_MOUSE_MOTION: {
                float x, y;
                SDL_GetMouseState(&x, &y);
                eventManager->Enqueue<MouseMovedEvent>(x, y);

                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                const SGScanCode sdlButtonCode = sdlEvent.button.button;
                const Key& sogeButton = KeyMapManager::GetInstance()->GetKeyFromScanCode(sdlButtonCode);

                KeyDetails* buttonDetails = sogeButton.GetDetails();
                FriendFuncAccessor accessor(KeyDetails::FriendlySetKeyState());
                accessor.Call(*buttonDetails, KeyState_KeyPressed);

                if (sdlEvent.key.repeat)
                    m_repeatCounter++;
                else
                    m_repeatCounter = 0;

                eventManager->Enqueue<MouseButtonPressedEvent>(sogeButton, m_repeatCounter);
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_UP: {
                const SGScanCode sdlButtonCode = sdlEvent.button.button;
                const Key& sogeButton = KeyMapManager::GetInstance()->GetKeyFromScanCode(sdlButtonCode);

                KeyDetails* buttonDetails = sogeButton.GetDetails();
                FriendFuncAccessor accessor(KeyDetails::FriendlySetKeyState());
                accessor.Call(*buttonDetails, KeyState_KeyReleased);

                eventManager->Enqueue<MouseButtonReleasedEvent>(sogeButton);
                break;
            }

            case SDL_EVENT_MOUSE_WHEEL: {
                float xOffset = sdlEvent.motion.x;
                float yOffset = sdlEvent.motion.y;
                eventManager->Enqueue<MouseWheelEvent>(xOffset, yOffset);
                break;
            }

            default:
                break;
            }
        }

        eventManager->DispatchQueue<MouseMovedEvent>();
        eventManager->DispatchQueue<MouseButtonPressedEvent>();
        eventManager->DispatchQueue<MouseButtonReleasedEvent>();
        eventManager->DispatchQueue<MouseWheelEvent>();
    }

    bool SDLMouse::IsButtonPressed(const Key aMouseButton)
    {
        return aMouseButton.GetKeyState() == KeyState_KeyPressed;
    }

    bool SDLMouse::IsButtonReleased(const Key aMouseButton)
    {
        return aMouseButton.GetKeyState() != KeyState_KeyPressed;
    }
}
