#ifndef SOGE_INPUT_IMPL_SDL_SDLKEYBOARD_HPP
#define SOGE_INPUT_IMPL_SDL_SDLKEYBOARD_HPP

#include "SOGE/Input/Device/Keyboard.hpp"
#include "SOGE/Input/Impl/SDL/SDLInputCore.hpp"


namespace soge
{
    class SDLKeyboard final : public Keyboard
    {
    private:
        SharedPtr<SDLInputCore> m_inputCoreSDL;
        std::uint32_t m_repeatCounter;

    public:
        explicit SDLKeyboard(const SharedPtr<SDLInputCore>& aInputCore);

        void Update() override;
        bool IsKeyPressed(Key aKeyName) override;
        bool IsKeyReleased(Key aKeyName) override;
    };

    typedef SDLKeyboard ImplKeyboard;
}

#endif // SOGE_INPUT_IMPL_SDL_SDLKEYBOARD_HPP
