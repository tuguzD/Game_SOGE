#include "sogepch.hpp"
#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    const Key Keys::BackSpace("BackSpace");
    const Key Keys::Tab("Tab");
    const Key Keys::Enter("Enter");
    const Key Keys::Pause("Pause");

    const Key Keys::CapsLock("CapsLock");
    const Key Keys::Escape("Escape");
    const Key Keys::SpaceBar("SpaceBar");
    const Key Keys::PageUp("PageUp");
    const Key Keys::PageDown("PageDown");
    const Key Keys::End("End");
    const Key Keys::Home("Home");

    const Key Keys::Left("Left");
    const Key Keys::Up("Up");
    const Key Keys::Right("Right");
    const Key Keys::Down("Down");

    const Key Keys::Insert("Insert");
    const Key Keys::Delete("Delete");

    const Key Keys::Zero("Zero");
    const Key Keys::One("One");
    const Key Keys::Two("Two");
    const Key Keys::Three("Three");
    const Key Keys::Four("Four");
    const Key Keys::Five("Five");
    const Key Keys::Six("Six");
    const Key Keys::Seven("Seven");
    const Key Keys::Eight("Eight");
    const Key Keys::Nine("Nine");

    const Key Keys::A("A");
    const Key Keys::B("B");
    const Key Keys::C("C");
    const Key Keys::D("D");
    const Key Keys::E("E");
    const Key Keys::F("F");
    const Key Keys::G("G");
    const Key Keys::H("H");
    const Key Keys::I("I");
    const Key Keys::J("J");
    const Key Keys::K("K");
    const Key Keys::L("L");
    const Key Keys::M("M");
    const Key Keys::N("N");
    const Key Keys::O("O");
    const Key Keys::P("P");
    const Key Keys::Q("Q");
    const Key Keys::R("R");
    const Key Keys::S("S");
    const Key Keys::T("T");
    const Key Keys::U("U");
    const Key Keys::V("V");
    const Key Keys::W("W");
    const Key Keys::X("X");
    const Key Keys::Y("Y");
    const Key Keys::Z("Z");

    const Key Keys::NumPadZero("NumPadZero");
    const Key Keys::NumPadOne("NumPadOne");
    const Key Keys::NumPadTwo("NumPadTwo");
    const Key Keys::NumPadThree("NumPadThree");
    const Key Keys::NumPadFour("NumPadFour");
    const Key Keys::NumPadFive("NumPadFive");
    const Key Keys::NumPadSix("NumPadSix");
    const Key Keys::NumPadSeven("NumPadSeven");
    const Key Keys::NumPadEight("NumPadEight");
    const Key Keys::NumPadNine("NumPadNine");

    const Key Keys::Multiply("Multiply");
    const Key Keys::Add("Add");
    const Key Keys::Subtract("Subtract");
    const Key Keys::Decimal("Decimal");
    const Key Keys::Divide("Divide");

    const Key Keys::F1("F1");
    const Key Keys::F2("F2");
    const Key Keys::F3("F3");
    const Key Keys::F4("F4");
    const Key Keys::F5("F5");
    const Key Keys::F6("F6");
    const Key Keys::F7("F7");
    const Key Keys::F8("F8");
    const Key Keys::F9("F9");
    const Key Keys::F10("F10");
    const Key Keys::F11("F11");
    const Key Keys::F12("F12");

    const Key Keys::NumLock("NumLock");
    const Key Keys::ScrollLock("ScrollLock");

    const Key Keys::LeftShift("LeftShift");
    const Key Keys::RightShift("RightShift");
    const Key Keys::LeftControl("LeftControl");
    const Key Keys::RightControl("RightControl");
    const Key Keys::LeftAlt("LeftAlt");
    const Key Keys::RightAlt("RightAlt");
    const Key Keys::LeftCommand("LeftCommand");
    const Key Keys::RightCommand("RightCommand");

    const Key Keys::Semicolon("Semicolon");
    const Key Keys::Equals("Equals");
    const Key Keys::Comma("Comma");
    const Key Keys::Underscore("Underscore");
    const Key Keys::Hyphen("Hyphen");
    const Key Keys::Period("Period");
    const Key Keys::Slash("Slash");
    const Key Keys::Tiled("Tiled");
    const Key Keys::LeftBracket("LeftBracket");
    const Key Keys::RightBracket("RightBracket");
    const Key Keys::LeftParantheses("LeftParanthesis");
    const Key Keys::RightParantheses("RightParantheses");
    const Key Keys::Apostrophe("Apostrophe");
    const Key Keys::Quote("Quote");

    Key::Key(const char* aKeyName) : m_keyName(aKeyName)
    {
    }

    bool Key::IsValid() const
    {
        return false;
    }

    const char* Key::ToCString() const
    {
        return m_keyName.c_str();
    }

    eastl::string Key::ToString() const
    {
        return m_keyName.eastl_str();
    }
}
