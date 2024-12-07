#include "sogepch.hpp"

#include "SOGE/Input/Impl/SDL/SDLInputCore.hpp"
#include "SOGE/Input/Impl/SDL/SDLKeyboard.hpp"

#include "SOGE/Containers/FriendFuncPtr.hpp"
#include "SOGE/Core/Engine.hpp"
#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Event/InputEvents.hpp"
#include "SOGE/Input/InputTypes.hpp"
#include "SOGE/Input/KeyMapManager.hpp"

#include <SDL3/SDL.h>


namespace soge
{
    SDLKeyboard::SDLKeyboard(SDLInputCore& aInputCore)
        : Keyboard("SDL Portable Keyboard"), m_inputCoreSDL(&aInputCore), m_repeatCounter(0)
    {
    }

    void SDLKeyboard::Update()
    {
        auto& keyMap = *m_inputCoreSDL->m_keyMapManager;
        auto& events = *m_inputCoreSDL->GetEventModule();

        for (const auto& sdlEvent : m_inputCoreSDL->m_sdlEventList)
        {
            switch (sdlEvent.type)
            {

            case SDL_EVENT_KEY_DOWN: {
                m_inputCoreSDL->m_isAnyButtonPressed = true;

                const SGScanCode sdlKeyCode = sdlEvent.key.key;
                const Key& sogeKey = keyMap.GetKeyFromScanCode(sdlKeyCode);

                KeyDetails* keyDetails = sogeKey.GetDetails();
                FriendFuncAccessor accessor(KeyDetails::FriendlySetKeyState());
                accessor.Call(*keyDetails, KeyState_KeyPressed);

                if (sdlEvent.key.repeat)
                    m_repeatCounter++;
                else
                    m_repeatCounter = 0;

                events.Enqueue<KeyPressedEvent>(sogeKey, m_repeatCounter);
                break;
            }

            case SDL_EVENT_KEY_UP: {
                m_inputCoreSDL->m_isAnyButtonPressed = false;

                const SGScanCode sdlKeyCode = sdlEvent.key.key;
                const Key& sogeKey = keyMap.GetKeyFromScanCode(sdlKeyCode);

                KeyDetails* keyDetails = sogeKey.GetDetails();
                FriendFuncAccessor accessor(KeyDetails::FriendlySetKeyState());
                accessor.Call(*keyDetails, KeyState_KeyReleased);

                events.Enqueue<KeyReleasedEvent>(sogeKey);
                break;
            }

            default:
                break;
            }
        }

        events.DispatchQueue<KeyPressedEvent>();
        events.DispatchQueue<KeyReleasedEvent>();
    }

    bool SDLKeyboard::IsKeyPressed(const Key aKeyName)
    {
        return aKeyName.GetKeyState() == KeyState_KeyPressed;
    }

    bool SDLKeyboard::IsKeyReleased(const Key aKeyName)
    {
        return aKeyName.GetKeyState() != KeyState_KeyPressed;
    }
}
