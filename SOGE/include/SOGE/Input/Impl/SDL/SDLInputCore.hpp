#ifndef SOGE_INPUT_IMPL_SDL_SDLINPUTCORE_HPP
#define SOGE_INPUT_IMPL_SDL_SDLINPUTCORE_HPP

#include "SOGE/Input/InputCore.hpp"
#include <SDL3/SDL_events.h>


namespace soge
{
    class SDLInputCore final : public InputCore,
        public eastl::enable_shared_from_this<SDLInputCore>
    {
        friend class SDLKeyboard;
        friend class SDLGamepad;
        friend class SDLMouse;

    private:
        bool m_isPauseUpdateRequested;
        bool m_isEndUpdateRequested;

    protected:
        eastl::list<SDL_Event> m_sdlEventList;
        bool m_isAnyButtonPressed;

    public:
        SDLInputCore();
        ~SDLInputCore();

        void LockInput(bool aLockInput) override;
        void UseRelativeMouseMode(bool aRelMouse) override;
        bool IsAnyButtonPressed() override;

        void BeginUpdateInput() override;
        void EndUpdateInput() override;
        void SetPauseUpdate(bool aIsPauseNeeded) override;

        Mouse* CreateMouse() override;
        Gamepad* CreateGamepad() override;
        Keyboard* CreateKeyboard() override;

    };

    typedef SDLInputCore ImplInputCore;

}

#endif // SOGE_INPUT_IMPL_SDL_SDLINPUTCORE_HPP
