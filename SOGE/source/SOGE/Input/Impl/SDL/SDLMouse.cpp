#include "sogepch.hpp"

#include "SOGE/Input/Impl/SDL/SDLMouse.hpp"

#include "SOGE/Containers/FriendFuncPtr.hpp"
#include "SOGE/Core/Engine.hpp"
#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Event/InputEvents.hpp"
#include "SOGE/Input/InputTypes.hpp"
#include "SOGE/Input/KeyMapManager.hpp"

#include <SDL3/SDL.h>


namespace soge
{
    SDLMouse::SDLMouse(SDLInputCore& aInputCore)
        : Mouse("SDL Portable Mouse"), m_inputCoreSDL(&aInputCore), m_repeatCounter(0)
    {
    }

    void SDLMouse::Update()
    {
        auto& keyMap = *m_inputCoreSDL->m_keyMapManager;
        auto& events = *m_inputCoreSDL->GetEventModule();

        for (const auto& sdlEvent : m_inputCoreSDL->m_sdlEventList)
        {
            switch (sdlEvent.type)
            {
            case SDL_EVENT_MOUSE_MOTION: {
                float x, y;
                SDL_GetMouseState(&x, &y);
                events.Enqueue<MouseMovedEvent>(x, y);

                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                const SGScanCode sdlButtonCode = sdlEvent.button.button;
                const Key& sogeButton = keyMap.GetKeyFromScanCode(sdlButtonCode);

                KeyDetails* buttonDetails = sogeButton.GetDetails();
                FriendFuncAccessor accessor(KeyDetails::FriendlySetKeyState());
                accessor.Call(*buttonDetails, KeyState_KeyPressed);

                if (sdlEvent.key.repeat)
                    m_repeatCounter++;
                else
                    m_repeatCounter = 0;

                events.Enqueue<MouseButtonPressedEvent>(sogeButton, m_repeatCounter);
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_UP: {
                const SGScanCode sdlButtonCode = sdlEvent.button.button;
                const Key& sogeButton = keyMap.GetKeyFromScanCode(sdlButtonCode);

                KeyDetails* buttonDetails = sogeButton.GetDetails();
                FriendFuncAccessor accessor(KeyDetails::FriendlySetKeyState());
                accessor.Call(*buttonDetails, KeyState_KeyReleased);

                events.Enqueue<MouseButtonReleasedEvent>(sogeButton);
                break;
            }

            case SDL_EVENT_MOUSE_WHEEL: {
                float xOffset = sdlEvent.motion.x;
                float yOffset = sdlEvent.motion.y;
                events.Enqueue<MouseWheelEvent>(xOffset, yOffset);
                break;
            }

            default:
                break;
            }
        }

        events.DispatchQueue<MouseMovedEvent>();
        events.DispatchQueue<MouseButtonPressedEvent>();
        events.DispatchQueue<MouseButtonReleasedEvent>();
        events.DispatchQueue<MouseWheelEvent>();
    }

}
