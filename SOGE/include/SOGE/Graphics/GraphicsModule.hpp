#ifndef SOGE_GRAPHICS_GRAPHICSMODULE_HPP
#define SOGE_GRAPHICS_GRAPHICSMODULE_HPP

#include "SOGE/Core/Module.hpp"
#include "SOGE/DI/Container.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/RenderGraph.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Utils/UUID.hpp"

#include <EASTL/functional.h>
#include <EASTL/hash_map.h>
#include <EASTL/vector.h>

#include <filesystem>


namespace soge
{
    class GraphicsModule : public Module
    {
    private:
        using Key = UUIDv4::UUID;
        using UniqueEntity = UniquePtr<GraphicsEntity>;

        Key CreateEntity(UniqueEntity aEntity);

        using Entities = eastl::hash_map<Key, UniqueEntity>;
        Entities m_entities;
        using EntitiesSpan = eastl::vector<eastl::reference_wrapper<GraphicsEntity>>;
        EntitiesSpan m_entitiesSpan;

        GraphicsCore* m_graphicsCore;
        RenderGraph* m_renderGraph;

    public:
        explicit GraphicsModule();

        template <typename T, typename... Args>
        [[nodiscard]]
        eastl::pair<T&, Key> CreateEntity(Args&&... args);

        [[nodiscard]]
        GraphicsEntity* GetEntity(const Key& aKey) const;

        UniqueEntity DestroyEntity(const Key& aKey);

        virtual void SetRenderTarget(const Window& aWindow);
        virtual void SetRenderGraph(RenderGraph& aRenderGraph);

        virtual void Update();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;
    };

    [[nodiscard]]
    std::filesystem::path GetCompiledShaderPath(const GraphicsCore& aCore, const std::filesystem::path& aSourcePath,
                                                eastl::string_view aEntryName = "");

    template <typename T, typename... Args>
    auto GraphicsModule::CreateEntity(Args&&... args) -> eastl::pair<T&, Key>
    {
        UniquePtr<T> entity = CreateUnique<T>(std::forward<Args>(args)...);
        T* entityPtr = entity.get();

        const auto key = CreateEntity(std::move(entity));
        return {*entityPtr, key};
    }
}

SOGE_DI_REGISTER_MODULE_NS(soge, GraphicsModule)

#endif // SOGE_GRAPHICS_GRAPHICSMODULE_HPP
