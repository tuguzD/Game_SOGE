#include "sogepch.hpp"

#include "SOGE/Graphics/GraphicsModule.hpp"

#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"
#include "SOGE/Graphics/GraphicsCompilePreproc.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

#include SOGE_ABS_COMPILED_GRAPHICS_IMPL_HEADER(SOGE/Graphics, GraphicsCore.hpp)


namespace soge
{
    GraphicsModule::GraphicsModule() : m_graphicsCore(nullptr)
    {
    }

    void GraphicsModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        SOGE_LOG_TITLE_DIVIDER(GRAPHICS MODULE)

        m_graphicsCore = &aContainer.Provide<ImplGraphicsCore>();
        aModuleManager.CreateModule<EventModule>();

        SOGE_INFO_LOG("Graphics module loaded...");
    }

    void GraphicsModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_graphicsCore = nullptr;

        SOGE_INFO_LOG("Graphics module unloaded...");
    }

    void GraphicsModule::SetRenderTarget(const Window& aWindow)
    {
        if (m_graphicsCore == nullptr)
        {
            return;
        }

        m_graphicsCore->SetRenderTarget(aWindow);
    }

    void GraphicsModule::Update(const float aDeltaTime)
    {
        if (m_graphicsCore == nullptr)
        {
            return;
        }

        m_graphicsCore->Update(aDeltaTime);
    }

    std::filesystem::path GetCompiledShaderPath(const GraphicsCore& aCore, const std::filesystem::path& aSourcePath,
                                                const eastl::string_view aEntryName)
    {
        std::filesystem::path destinationPath{aSourcePath};

        if (!aEntryName.empty())
        {
            const auto sourceFilename = aSourcePath.stem();
            const auto destinationFilename = fmt::format("{}_{}", sourceFilename.generic_string(), aEntryName.data());
            destinationPath.replace_filename(destinationFilename);
        }

        const auto extension = aCore.GetCompiledShaderExtension();
        destinationPath.replace_extension(extension.data());

        return destinationPath;
    }
}
