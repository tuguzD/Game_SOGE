#include "sogepch.hpp"
#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"

#include "SOGE/Sound/SoundModule.hpp"
#include "SOGE/Sound/SoundCompilePreproc.hpp"

#include SOGE_ABS_COMPILED_SOUNDSYSTEM_IMPL_HEADER(SOGE/Sound, SoundCore.hpp);


namespace soge
{
    SoundModule::SoundModule()
    {
    }

    void SoundModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        aModuleManager.CreateModule<EventModule>();
        m_soundCore = &aContainer.Provide<ImplSoundCore>();

        SOGE_INFO_LOG("Sound module loaded...");
    }

    void SoundModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_soundCore = nullptr;
        SOGE_INFO_LOG("Sound module unloaded...");
    }

    void SoundModule::LoadSoundResource(SoundResource& aSoundResource) const
    {
        m_soundCore->LoadSoundResource(aSoundResource);
    }

    void SoundModule::PlaySoundResource(SoundResource& aSoundResource) const
    {
        m_soundCore->PlaySoundResource(aSoundResource);
    }

    void SoundModule::Update() const
    {
        m_soundCore->BeginUpdateSound();
    }
}
