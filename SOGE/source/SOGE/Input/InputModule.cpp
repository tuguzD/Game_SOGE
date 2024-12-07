#include "sogepch.hpp"

#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/Input/InputModule.hpp"
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

    void InputModule::Update() const
    {
        m_inputCore->BeginUpdateInput();
    }
}
