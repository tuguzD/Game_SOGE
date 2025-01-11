#include "sogepch.hpp"

#include "SOGE/Graphics/GraphicsModule.hpp"

#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"
#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Graphics/GraphicsCompilePreproc.hpp"
#include "SOGE/Graphics/TriangleEntity.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

#include SOGE_ABS_COMPILED_GRAPHICS_IMPL_HEADER(SOGE/Graphics, GraphicsCore.hpp)


namespace soge
{
    GraphicsModule::GraphicsModule() : m_graphicsCore{nullptr}, m_renderGraph{nullptr}
    {
    }

    void GraphicsModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        SOGE_LOG_TITLE_DIVIDER(GRAPHICS MODULE)

        aModuleManager.CreateModule<EventModule>();
        aContainer.Create<ImplGraphicsCore>();

        m_graphicsCore = &aContainer.Provide<GraphicsCore>();

        SOGE_INFO_LOG("Graphics module loaded...");
    }

    void GraphicsModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_entitiesSpan.clear();
        m_entities.clear();

        m_renderGraph = nullptr;
        m_graphicsCore = nullptr;

        SOGE_INFO_LOG("Graphics module unloaded...");
    }

    auto GraphicsModule::CreateEntity(UniqueEntity aEntity) -> Key
    {
        const auto key = UUID::Generate();

        const auto [iter, created] = m_entities.try_emplace(key, std::move(aEntity));
        if (created)
        {
            m_entitiesSpan.push_back(*iter->second);
        }

        return key;
    }

    GraphicsEntity* GraphicsModule::GetEntity(const Key& aKey) const
    {
        const auto iter = m_entities.find(aKey);
        if (iter == m_entities.end())
        {
            return nullptr;
        }

        return iter->second.get();
    }

    auto GraphicsModule::DestroyEntity(const Key& aKey) -> UniqueEntity
    {
        const auto iter = m_entities.find(aKey);
        if (iter == m_entities.end())
        {
            return UniqueEntity{};
        }

        UniqueEntity entity = std::move(iter->second);
        m_entities.erase(iter);
        m_entitiesSpan.erase(eastl::remove_if(m_entitiesSpan.begin(), m_entitiesSpan.end(),
                                              [&entity](auto&& aItem) { return entity.get() == &aItem.get(); }),
                             m_entitiesSpan.end());

        return entity;
    }

    void GraphicsModule::SetRenderTarget(const Window& aWindow)
    {
        if (m_graphicsCore == nullptr)
        {
            return;
        }

        m_graphicsCore->SetRenderTarget(aWindow);
    }

    void GraphicsModule::SetRenderGraph(RenderGraph& aRenderGraph)
    {
        m_renderGraph = &aRenderGraph;
    }

    void GraphicsModule::Update()
    {
        if (m_graphicsCore == nullptr && m_renderGraph == nullptr)
        {
            return;
        }

        m_graphicsCore->Update(*m_renderGraph, m_entitiesSpan);
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
