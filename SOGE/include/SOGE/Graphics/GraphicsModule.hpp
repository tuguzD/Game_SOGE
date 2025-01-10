#ifndef SOGE_GRAPHICS_GRAPHICSMODULE_HPP
#define SOGE_GRAPHICS_GRAPHICSMODULE_HPP

#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/RenderGraph.hpp"

#include <filesystem>


namespace soge
{
    class GraphicsModule : public Module
    {
    private:
        GraphicsCore* m_graphicsCore;
        UniquePtr<RenderGraph> m_renderGraph;

    public:
        explicit GraphicsModule();

        virtual void SetRenderTarget(const Window& aWindow);

        template <typename T, typename... Args>
        void SetRenderGraph(Args&&... args);

        virtual void Update(float aDeltaTime);

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;
    };

    [[nodiscard]]
    std::filesystem::path GetCompiledShaderPath(const GraphicsCore& aCore, const std::filesystem::path& aSourcePath,
                                                eastl::string_view aEntryName = "");

    template <typename T, typename... Args>
    void GraphicsModule::SetRenderGraph(Args&&... args)
    {
        if (m_graphicsCore == nullptr)
        {
            return;
        }

        m_renderGraph = CreateUnique<T>(*m_graphicsCore, std::forward<Args>(args)...);
        // TODO: injection of abstract class does not work somehow (but it was overridden); fix to work
        // m_renderGraph = &aContainer.Provide<SimpleRenderGraph>();
    }
}

SOGE_DI_REGISTER_MODULE_NS(soge, GraphicsModule)

#endif // SOGE_GRAPHICS_GRAPHICSMODULE_HPP
