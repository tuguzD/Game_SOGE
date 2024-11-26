#ifndef SOGE_INPUT_IMPL_SDL_INPUTCORESDL_HPP
#define SOGE_INPUT_IMPL_SDL_INPUTCORESDL_HPP

#include "SOGE/Input/InputCore.hpp"
#include <SDL3/SDL_events.h>


namespace soge
{
    class InputCoreSDL : public InputCore
    {
        friend class KeyboardSDL;
        friend class GamepadSDL;
        friend class MouseSDL;

    private:
        bool m_isPauseUpdateRequested;
        bool m_isEndUpdateRequested;

    protected:
        eastl::list<SDL_Event> m_sdlEventList;

    public:
        InputCoreSDL();
        ~InputCoreSDL();

        void LockInput(bool aLockInput) override;
        void UseRelativeMouseMode(bool aRelMouse) override;

        void BeginUpdateInput() override;
        void EndUpdateInput() override;
        void SetPauseUpdate(bool aIsPauseNeeded) override;

        Mouse* CreateMouse() override;
        Gamepad* CreateGamepad() override;
        Keyboard* CreateKeyboard() override;

    };
}

#endif // SOGE_INPUT_IMPL_SDL_INPUTCORESDL_HPP
