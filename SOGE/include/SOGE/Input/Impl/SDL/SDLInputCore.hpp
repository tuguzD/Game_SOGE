#ifndef SOGE_INPUT_IMPL_SDL_SDLINPUTCORE_HPP
#define SOGE_INPUT_IMPL_SDL_SDLINPUTCORE_HPP

#include "SOGE/Input/InputCore.hpp"

#include <SDL3/SDL_events.h>


namespace soge
{
    class SDLInputCore final : public InputCore
    {
    private:
        friend class SDLKeyboard;
        friend class SDLGamepad;
        friend class SDLMouse;
        friend class SDLKeyMapManager;

        eastl::list<SDL_Event> m_sdlEventList;

        bool m_isPauseUpdateRequested;
        bool m_isEndUpdateRequested;
        bool m_isAnyButtonPressed;

    public:
        explicit SDLInputCore(EventModule* aEventModule);

        void LockInput(bool aLockInput) override;
        void UseRelativeMouseMode(bool aRelMouse) override;
        bool IsAnyButtonPressed() override;

        void BeginUpdateInput() override;
        void EndUpdateInput() override;
        void SetPauseUpdate(bool aIsPauseNeeded) override;
    };

    using ImplInputCore = SDLInputCore;
}

#endif // SOGE_INPUT_IMPL_SDL_SDLINPUTCORE_HPP
