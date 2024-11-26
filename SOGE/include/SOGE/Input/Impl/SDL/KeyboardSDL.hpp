#ifndef SOGE_INPUT_IMPL_SDL_KEYBOARDSDL_HPP
#define SOGE_INPUT_IMPL_SDL_KEYBOARDSDL_HPP

#include "SOGE/Input/Impl/SDL/InputCoreSDL.hpp"
#include "SOGE/Core/Event/Event.hpp"
#include "SOGE/Input/Keyboard.hpp"

namespace soge
{
    class KeyboardSDL final : public Keyboard
    {
    private:
        DefaultEventCallback m_fnInputCallback;
        SharedPtr<InputCoreSDL> m_inputCoreSDL;
        eastl::vector<bool> m_keyArray;

    public:
        KeyboardSDL(InputCoreSDL* aInputCore);
        ~KeyboardSDL() = default;

        void Update() override;

        bool IsKeyPressed(Key aKeyName) override;
        bool IsKeyReleased(Key aKeyName) override;

        Key GetPressedKey() override;
        Key GetReleasedKey() override;

        bool IsAnyKeyPressed() override;
        bool IsAnyKeyReleased() override;

    };
}

#endif // SOGE_INPUT_IMPL_SDL_KEYBOARDSDL_HPP
