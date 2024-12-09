#include "sogepch.hpp"
#include "SOGE/Graphics/GraphicsModule.hpp"

#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"
#include "SOGE/Graphics/GraphicsCompilePreproc.hpp"

#include SOGE_ABS_COMPILED_GRAPHICS_IMPL_HEADER(SOGE/Graphics, GraphicsCore.hpp);


namespace soge
{
    GraphicsModule::GraphicsModule() : m_graphicsCore(nullptr)
    {
    }

    void GraphicsModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        aModuleManager.CreateModule<EventModule>();
        m_graphicsCore = &aContainer.Provide<ImplGraphicsCore>();

        SOGE_INFO_LOG("Graphics module loaded...");
    }

    void GraphicsModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_graphicsCore = nullptr;
        SOGE_INFO_LOG("Graphics module unloaded...");
    }
}
