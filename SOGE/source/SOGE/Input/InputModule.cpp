#include "sogepch.hpp"

#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/Input/InputModule.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

// clang-format off
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, InputCore.hpp)
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, Keyboard.hpp)
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, Mouse.hpp)
// clang-format on


namespace soge
{
    InputModule::InputModule() : m_eventModule(nullptr)
    {
        Keys::Initialize();

        m_inputCore.reset(new ImplInputCore());
        m_keyboard.reset(m_inputCore->CreateKeyboard());
        m_mouse.reset(m_inputCore->CreateMouse());
    }

    void InputModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_eventModule = aModuleManager.GetModule<EventModule>();
        if (m_eventModule)
        {
            auto update = [this](const UpdateEvent&) { this->Update(); };
            m_updateEventHandle = m_eventModule->PushBack<UpdateEvent>(update);
        }

        SOGE_INFO_LOG("Input module loaded...");
    }

    void InputModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        if (m_eventModule)
        {
            m_eventModule->Remove(m_updateEventHandle);
        }

        SOGE_INFO_LOG("Input module unloaded...");
    }

    void InputModule::Update() const
    {
        m_inputCore->BeginUpdateInput();
        m_keyboard->Update();
        m_mouse->Update();
    }
}
