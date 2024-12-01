#include "sogepch.hpp"
#include "SOGE/Input/Impl/SDL/SDLMouse.hpp"
#include "SOGE/Core/Engine.hpp"


namespace soge
{
    SDLMouse::SDLMouse(SharedPtr<SDLInputCore> aInputCore) : Mouse("SDL Portable Mouse"), m_inputCoreSDL(aInputCore)
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
                SOGE_INFO_LOG("Mouse motion event");
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                SOGE_INFO_LOG("Button down event");
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_UP: {
                SOGE_INFO_LOG("Button up event");
                break;
            }

            case SDL_EVENT_MOUSE_WHEEL: {
                SOGE_INFO_LOG("Wheel event");
                break;
            }

            default:
                break;
            }
        }
    }

    bool SDLMouse::IsButtonPressed()
    {
        return false;
    }

    bool SDLMouse::IsButtonReleased()
    {
        return false;
    }
}
