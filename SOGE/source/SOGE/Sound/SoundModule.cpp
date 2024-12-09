#include "sogepch.hpp"
#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/Sound/SoundModule.hpp"
#include "SOGE/Sound/SoundCompilePreproc.hpp"

#include SOGE_ABS_COMPILED_SOUNDSYSTEM_HEADER(SOGE/Sound, SoundCore.hpp);


namespace soge
{
    SoundModule::SoundModule()
    {
    }

    void SoundModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_eventModule = aModuleManager.GetModule<EventModule>();
        if (m_eventModule)
        {
            auto update = [this](const UpdateEvent&) { this->Update(); };
            m_updateEventHandle = m_eventModule->PushBack<UpdateEvent>(update);
            m_soundCore = CreateUnique<ImplSoundCore>(m_eventModule);
        }

        SOGE_INFO_LOG("Sound module loaded...");
    }

    void SoundModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        if (m_eventModule)
        {
            m_eventModule->Remove(m_updateEventHandle);
            m_soundCore.reset();
        }

        SOGE_INFO_LOG("Sound module unloaded...");
    }

    void SoundModule::Update() const
    {

    }
}
