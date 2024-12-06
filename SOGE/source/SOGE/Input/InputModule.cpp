#include "sogepch.hpp"

#include "SOGE/Input/InputModule.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, InputCore.hpp)
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, Keyboard.hpp)
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, Mouse.hpp)


namespace soge
{
    InputModule::InputModule()
    {
        Keys::Initialize();

        m_inputCore.reset(new ImplInputCore());
        m_keyboard.reset(m_inputCore->CreateKeyboard());
        m_mouse.reset(m_inputCore->CreateMouse());
    }

    void InputModule::Update()
    {
        m_inputCore->BeginUpdateInput();
        m_keyboard->Update();
        m_mouse->Update();
    }

    void InputModule::Load(di::Container& aContainer)
    {
        SOGE_INFO_LOG("Input module loaded...");
    }

    void InputModule::Unload(di::Container& aContainer)
    {
        SOGE_INFO_LOG("Input module unloaded...");
    }
}
