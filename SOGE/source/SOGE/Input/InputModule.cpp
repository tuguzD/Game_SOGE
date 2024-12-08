#include "sogepch.hpp"

#include "SOGE/Input/InputModule.hpp"

#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"
#include "SOGE/System/Impl/SDL/SDLContext.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

// clang-format off
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, InputCore.hpp)
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
            m_inputCore = CreateUnique<ImplInputCore>(m_eventModule);

            // TODO: move this into impl input core
            aContainer.Create<SDLContext>();
        }

        SOGE_INFO_LOG("Input module loaded...");
    }

    void InputModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        if (m_eventModule)
        {
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
