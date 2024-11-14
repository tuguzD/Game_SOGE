#ifndef SOGE_INPUT_INPUTTYPES_HPP
#define SOGE_INPUT_INPUTTYPES_HPP

#include "SOGE/Containers/LWString.hpp"


namespace soge
{
    class Key
    {
    private:
        LWString m_keyName;

    public:
        Key(const char* aKeyName);
        ~Key() = default;

        bool IsValid() const;

        const char* ToCString() const;
        eastl::string ToString() const;
    };

    struct Keys
    {
        static const Key AnyKey;

        static const Key BackSpace;
        static const Key Tab;
        static const Key Enter;
        static const Key Pause;

        static const Key CapsLock;
        static const Key Escape;
        static const Key SpaceBar;
        static const Key PageUp;
        static const Key PageDown;
        static const Key End;
        static const Key Home;

        static const Key Left;
        static const Key Up;
        static const Key Right;
        static const Key Down;

        static const Key Insert;
        static const Key Delete;

        static const Key Zero;
        static const Key One;
        static const Key Two;
        static const Key Three;
        static const Key Four;
        static const Key Five;
        static const Key Six;
        static const Key Seven;
        static const Key Eight;
        static const Key Nine;

        static const Key A;
        static const Key B;
        static const Key C;
        static const Key D;
        static const Key E;
        static const Key F;
        static const Key G;
        static const Key H;
        static const Key I;
        static const Key J;
        static const Key K;
        static const Key L;
        static const Key M;
        static const Key N;
        static const Key O;
        static const Key P;
        static const Key Q;
        static const Key R;
        static const Key S;
        static const Key T;
        static const Key U;
        static const Key V;
        static const Key W;
        static const Key X;
        static const Key Y;
        static const Key Z;

        static const Key NumPadZero;
        static const Key NumPadOne;
        static const Key NumPadTwo;
        static const Key NumPadThree;
        static const Key NumPadFour;
        static const Key NumPadFive;
        static const Key NumPadSix;
        static const Key NumPadSeven;
        static const Key NumPadEight;
        static const Key NumPadNine;

        static const Key Multiply;
        static const Key Add;
        static const Key Subtract;
        static const Key Decimal;
        static const Key Divide;

        static const Key F1;
        static const Key F2;
        static const Key F3;
        static const Key F4;
        static const Key F5;
        static const Key F6;
        static const Key F7;
        static const Key F8;
        static const Key F9;
        static const Key F10;
        static const Key F11;
        static const Key F12;

        static const Key NumLock;
        static const Key ScrollLock;

        static const Key LeftShift;
        static const Key RightShift;
        static const Key LeftControl;
        static const Key RightControl;
        static const Key LeftAlt;
        static const Key RightAlt;
        static const Key LeftCommand;
        static const Key RightCommand;

        static const Key Semicolon;
        static const Key Equals;
        static const Key Comma;
        static const Key Underscore;
        static const Key Hyphen;
        static const Key Period;
        static const Key Slash;
        static const Key Tiled;
        static const Key LeftBracket;
        static const Key Backslash;
        static const Key RightBracket;
        static const Key Apostrophe;

        static const Key Ampersand;
        static const Key Asterix;
        static const Key Caret;
        static const Key Colon;
        static const Key Dollar;
        static const Key Exclamation;
        static const Key LeftParantheses;
        static const Key RightParantheses;
        static const Key Quote;
    };
}

#endif // !SOGE_INPUT_INPUTTYPES_HPP
