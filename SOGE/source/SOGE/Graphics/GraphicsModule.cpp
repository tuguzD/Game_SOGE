#include "sogepch.hpp"

#include "SOGE/Graphics/GraphicsModule.hpp"

#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"
#include "SOGE/Graphics/GraphicsCompilePreproc.hpp"
#include "SOGE/Graphics/TriangleEntity.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

#include SOGE_ABS_COMPILED_GRAPHICS_IMPL_HEADER(SOGE/Graphics, GraphicsCore.hpp)


namespace soge
{
    GraphicsModule::GraphicsModule() : m_container{nullptr}, m_graphicsCore{nullptr}, m_renderGraph{nullptr}
    {
    }

    void GraphicsModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        SOGE_LOG_TITLE_DIVIDER(GRAPHICS MODULE)

        aModuleManager.CreateModule<EventModule>();
        aContainer.Create<ImplGraphicsCore>();

        m_container = &aContainer;
        m_graphicsCore = &aContainer.Provide<GraphicsCore>();

        SOGE_INFO_LOG("Graphics module loaded...");
    }

    void GraphicsModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_graphicsEntity = nullptr;
        m_renderGraph = nullptr;
        m_graphicsCore = nullptr;
        m_container = nullptr;

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

    void GraphicsModule::Update()
    {
        if (m_graphicsCore == nullptr && m_renderGraph == nullptr)
        {
            return;
        }

        if (m_graphicsEntity == nullptr)
        {
            using Vertex = TriangleEntity::Vertex;

            constexpr std::array vertices{
                Vertex{
                    .m_position = glm::vec4{-0.5f, 0.5f, 0.0f, 0.0f},
                    .m_color = glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},
                },
                Vertex{
                    .m_position = glm::vec4{0.5f, 0.5f, 0.0f, 0.0f},
                    .m_color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},
                },
                Vertex{
                    .m_position = glm::vec4{0.0f, -0.5f, 0.0f, 0.0f},
                    .m_color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
                },
            };

            auto entity = m_container->Provide<TriangleEntity>();
            entity.UpdateVertices(vertices);

            m_graphicsEntity = CreateUnique<TriangleEntity>(std::move(entity));
        }
        auto graphicsEntityRef = eastl::ref(*m_graphicsEntity);
        const GraphicsCore::Entities entities{&graphicsEntityRef, 1};
        m_graphicsCore->Update(*m_renderGraph, entities);
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
