#include "sogepch.hpp"
#include "SOGE/Input/InputManager.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input,InputCore.hpp)
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input,Keyboard.hpp)
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input,Mouse.hpp)


namespace soge
{
    InputManager::InputManager()
    {
        Keys::Initialize();

        m_inputCore.reset(new ImplInputCore());
        m_keyboard.reset(m_inputCore->CreateKeyboard());
        m_mouse.reset(m_inputCore->CreateMouse());
    }

    void InputManager::Update()
    {
        m_inputCore->BeginUpdateInput();
        m_keyboard->Update();
    }
}
