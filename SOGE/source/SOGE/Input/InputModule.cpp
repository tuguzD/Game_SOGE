#include "sogepch.hpp"

#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/Input/InputModule.hpp"
#include "SOGE/Input/InputCompilePreproc.hpp"

// clang-format off
#include SOGE_ABS_COMPILED_INPUTSYSTEM_HEADER(SOGE/Input, InputCore.hpp)
#include SOGE_ABS_COMPILED_INPUTSYSTEM_HEADER(SOGE/Input, Keyboard.hpp)
#include SOGE_ABS_COMPILED_INPUTSYSTEM_HEADER(SOGE/Input, Gamepad.hpp)
#include SOGE_ABS_COMPILED_INPUTSYSTEM_HEADER(SOGE/Input, Mouse.hpp)
// clang-format on



namespace soge
{
    InputModule::InputModule() : m_eventModule(nullptr)
    {
        Keys::Initialize();
    }

    void InputModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_eventModule = aModuleManager.GetModule<EventModule>();
        if (m_eventModule)
        {
            auto update = [this](const UpdateEvent&) { this->Update(); };
            m_updateEventHandle = m_eventModule->PushBack<UpdateEvent>(update);
            m_inputCore = CreateUnique<ImplInputCore>(m_eventModule);
        }

        SOGE_INFO_LOG("Input module loaded...");
    }

    void InputModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        if (m_eventModule)
        {
            m_eventModule->Remove(m_updateEventHandle);

            m_inputCore.reset();
        }

        SOGE_INFO_LOG("Input module unloaded...");
    }

    bool InputModule::IsKeyPressed(const Key& aKey) const
    {
        return m_inputCore->IsKeyPressed(aKey);
    }

    bool InputModule::IsKeyReleased(const Key& aKey) const
    {
        return m_inputCore->IsKeyReleased(aKey);
    }

    void InputModule::Update() const
    {
        m_inputCore->BeginUpdateInput();
    }

    Keyboard* InputModule::GetKeyboard() const
    {
        return m_inputCore->GetKeyboard();
    }

    Gamepad* InputModule::GetGamepad() const
    {
        return m_inputCore->GetGamepad();
    }

    Mouse* InputModule::GetMouse() const
    {
        return m_inputCore->GetMouse();
    }
}
