#ifndef SOGE_GRAPHICS_GRAPHICSMODULE_HPP
#define SOGE_GRAPHICS_GRAPHICSMODULE_HPP

#include "SOGE/DI/Container.hpp"
#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/RenderGraph.hpp"

#include <filesystem>


namespace soge
{
    class GraphicsModule : public Module
    {
    private:
        di::Container* m_container;
        GraphicsCore* m_graphicsCore;
        RenderGraph* m_renderGraph;

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
        if (m_graphicsCore == nullptr && m_container == nullptr)
        {
            return;
        }

        m_container->Create<T>(std::forward<Args>(args)...);
        m_renderGraph = &m_container->Provide<T>();
    }
}

SOGE_DI_REGISTER_MODULE_NS(soge, GraphicsModule)

#endif // SOGE_GRAPHICS_GRAPHICSMODULE_HPP
