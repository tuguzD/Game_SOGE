#ifndef SOGE_INPUT_IMPL_SDL_KEYBOARDSDL_HPP
#define SOGE_INPUT_IMPL_SDL_KEYBOARDSDL_HPP

#include "SOGE/Input/Keyboard.hpp"

namespace soge
{
    class KeyboardSDL final : public Keyboard
    {
    private:
        eastl::vector<bool> m_keyArray;

    public:
        KeyboardSDL();
        ~KeyboardSDL() = default;

        bool IsKeyPressed(Key aKeyName) override;
        bool IsKeyReleased(Key aKeyName) override;

        Key GetPressedKey() override;
        Key GetReleasedKey() override;

        bool IsAnyKeyPressed() override;
        bool IsAnyKeyReleased() override;

    };
}

#endif // SOGE_INPUT_IMPL_SDL_KEYBOARDSDL_HPP
