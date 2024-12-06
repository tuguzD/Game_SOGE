#include "sogepch.hpp"

#include "SOGE/Input/Impl/SDL/SDLKeyMapManager.hpp"
#include "SOGE/Input/InputTypes.hpp"

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>


namespace soge
{
    void impl::SDLKeyMapManager::SetupKeyMappings()
    {
        SetScanCode(SDL_BUTTON_LEFT, Keys::LeftMouseButton);
        SetScanCode(SDL_BUTTON_MIDDLE, Keys::MiddleMouseButton);
        SetScanCode(SDL_BUTTON_RIGHT, Keys::RightMouseButton);
        SetScanCode(SDL_BUTTON_X1, Keys::ThumbMouseButton);
        SetScanCode(SDL_BUTTON_X2, Keys::ThumbMouseButton2);

        SetScanCode(SDLK_0, Keys::Zero);
        SetScanCode(SDLK_1, Keys::One);
        SetScanCode(SDLK_2, Keys::Two);
        SetScanCode(SDLK_3, Keys::Three);
        SetScanCode(SDLK_4, Keys::Four);
        SetScanCode(SDLK_5, Keys::Five);
        SetScanCode(SDLK_6, Keys::Six);
        SetScanCode(SDLK_7, Keys::Seven);
        SetScanCode(SDLK_8, Keys::Eight);
        SetScanCode(SDLK_9, Keys::Nine);

        SetScanCode(SDLK_F1, Keys::F1);
        SetScanCode(SDLK_F2, Keys::F2);
        SetScanCode(SDLK_F3, Keys::F3);
        SetScanCode(SDLK_F4, Keys::F4);
        SetScanCode(SDLK_F5, Keys::F5);
        SetScanCode(SDLK_F6, Keys::F6);
        SetScanCode(SDLK_F7, Keys::F7);
        SetScanCode(SDLK_F8, Keys::F8);
        SetScanCode(SDLK_F9, Keys::F9);
        SetScanCode(SDLK_F10, Keys::F10);
        SetScanCode(SDLK_F11, Keys::F11);
        SetScanCode(SDLK_F12, Keys::F12);

        SetScanCode(SDLK_A, Keys::A);
        SetScanCode(SDLK_B, Keys::B);
        SetScanCode(SDLK_C, Keys::C);
        SetScanCode(SDLK_D, Keys::D);
        SetScanCode(SDLK_E, Keys::E);
        SetScanCode(SDLK_F, Keys::F);
        SetScanCode(SDLK_G, Keys::G);
        SetScanCode(SDLK_H, Keys::H);
        SetScanCode(SDLK_I, Keys::I);
        SetScanCode(SDLK_J, Keys::J);
        SetScanCode(SDLK_K, Keys::K);
        SetScanCode(SDLK_L, Keys::L);
        SetScanCode(SDLK_M, Keys::M);
        SetScanCode(SDLK_N, Keys::N);
        SetScanCode(SDLK_O, Keys::O);
        SetScanCode(SDLK_P, Keys::P);
        SetScanCode(SDLK_Q, Keys::Q);
        SetScanCode(SDLK_R, Keys::R);
        SetScanCode(SDLK_S, Keys::S);
        SetScanCode(SDLK_T, Keys::T);
        SetScanCode(SDLK_U, Keys::U);
        SetScanCode(SDLK_V, Keys::V);
        SetScanCode(SDLK_W, Keys::W);
        SetScanCode(SDLK_X, Keys::X);
        SetScanCode(SDLK_Y, Keys::Y);
        SetScanCode(SDLK_Z, Keys::Z);

        SetScanCode(SDLK_BACKSPACE, Keys::BackSpace);
        SetScanCode(SDLK_TAB, Keys::Tab);
        SetScanCode(SDLK_RETURN, Keys::Enter);
        SetScanCode(SDLK_PAUSE, Keys::Pause);

        SetScanCode(SDLK_CAPSLOCK, Keys::CapsLock);
        SetScanCode(SDLK_ESCAPE, Keys::Escape);
        SetScanCode(SDLK_PAGEUP, Keys::PageUp);
        SetScanCode(SDLK_PAGEDOWN, Keys::PageDown);
        SetScanCode(SDLK_END, Keys::End);
        SetScanCode(SDLK_HOME, Keys::Home);

        SetScanCode(SDLK_LEFT, Keys::Left);
        SetScanCode(SDLK_UP, Keys::Up);
        SetScanCode(SDLK_RIGHT, Keys::Right);
        SetScanCode(SDLK_DOWN, Keys::Down);

        SetScanCode(SDLK_INSERT, Keys::Insert);
        SetScanCode(SDLK_DELETE, Keys::Delete);

        SetScanCode(SDLK_LALT, Keys::LeftAlt);
        SetScanCode(SDLK_RALT, Keys::RightAlt);
        SetScanCode(SDLK_LCTRL, Keys::LeftControl);
        SetScanCode(SDLK_RCTRL, Keys::RightControl);
        SetScanCode(SDLK_LSHIFT, Keys::LeftShift);
        SetScanCode(SDLK_RSHIFT, Keys::RightShift);
        SetScanCode(SDLK_NUMLOCKCLEAR, Keys::NumLock);

        SetScanCode(SDLK_SEMICOLON, Keys::Semicolon);       // ;
        SetScanCode(SDLK_EQUALS, Keys::Equals);             // =
        SetScanCode(SDLK_COMMA, Keys::Comma);               // ,
        SetScanCode(SDLK_MINUS, Keys::Hyphen);              // -
        SetScanCode(SDLK_PERIOD, Keys::Period);             // .
        SetScanCode(SDLK_SLASH, Keys::Slash);               // /
        SetScanCode(SDLK_TILDE, Keys::Tilde);               // ~
        SetScanCode(SDLK_LEFTBRACKET, Keys::LeftBracket);   // [
        SetScanCode(SDLK_RIGHTBRACKET, Keys::RightBracket); // ]
        SetScanCode(SDLK_BACKSLASH, Keys::Backslash);       // \'
        SetScanCode(SDLK_APOSTROPHE, Keys::Apostrophe);     // '
        SetScanCode(SDLK_SPACE, Keys::SpaceBar);            // ` `

        SetScanCode(SDLK_AMPERSAND, Keys::Ampersand);         // &
        SetScanCode(SDLK_ASTERISK, Keys::Asterix);            // *
        SetScanCode(SDLK_CARET, Keys::Caret);                 // ^
        SetScanCode(SDLK_COLON, Keys::Colon);                 // :
        SetScanCode(SDLK_DOLLAR, Keys::Dollar);               // $
        SetScanCode(SDLK_EXCLAIM, Keys::Exclamation);         // !
        SetScanCode(SDLK_LEFTPAREN, Keys::LeftParenthesis);   // (
        SetScanCode(SDLK_RIGHTPAREN, Keys::RightParenthesis); // )
        SetScanCode(SDLK_DBLAPOSTROPHE, Keys::Quote);         // "
        SetScanCode(SDLK_UNDERSCORE, Keys::Underscore);       // _
    }
}
