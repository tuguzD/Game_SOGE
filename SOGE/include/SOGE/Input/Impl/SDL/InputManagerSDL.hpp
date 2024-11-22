#ifndef SOGE_INPUT_IMPL_SDL_INPUTMANAGERSDL_HPP
#define SOGE_INPUT_IMPL_SDL_INPUTMANAGERSDL_HPP

#include "SOGE/Input/InputManager.hpp"
#include "SOGE/Core/EventManager.hpp"

#include <SDL3/SDL_events.h>


namespace soge
{
    class InputManagerSDL : public InputManager
    {
    public:
        eastl::list<SDL_Event> m_eventList;

    public:
        InputManagerSDL();
        ~InputManagerSDL();

        void LockInput(bool aLockInput) override;
        void UseRelativeMouseMode(bool aRelMouse) override;
        void BeginUpdateInput() override;
        void EndUpdateInput() override;

        Keyboard* CreateKeyboard() override;

    };
}

#endif // !SOGE_INPUT_IMPL_SDL_INPUTMANAGERSDL_HPP
