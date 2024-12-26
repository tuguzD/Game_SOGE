#ifndef SOGE_GRAPHICS_GRAPHICSMODULE_HPP
#define SOGE_GRAPHICS_GRAPHICSMODULE_HPP

#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"

#include <filesystem>


namespace soge
{
    class GraphicsModule : public Module
    {
    private:
        GraphicsCore* m_graphicsCore;

    public:
        explicit GraphicsModule();

        virtual void SetRenderTarget(const Window& aWindow);
        virtual void Update(float aDeltaTime);

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;
    };

    [[nodiscard]]
    std::filesystem::path GetCompiledShaderPath(const GraphicsCore& aCore, const std::filesystem::path& aSourcePath,
                                                eastl::string_view aEntryName = "");
}

SOGE_DI_REGISTER_MODULE_NS(soge, GraphicsModule)

#endif // SOGE_GRAPHICS_GRAPHICSMODULE_HPP
