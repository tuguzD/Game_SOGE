#ifndef SOGE_GRAPHICS_ENTITIES_STATICMESHENTITY_HPP
#define SOGE_GRAPHICS_ENTITIES_STATICMESHENTITY_HPP

#include "SOGE/Graphics/Entities/GeometryEntity.hpp"
#include "SOGE/Graphics/GraphicsModule.hpp"

#include <cppfs/FilePath.h>


namespace soge
{
    class StaticMeshEntity : public GraphicsEntity
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsPipeline> m_pipeline;
        eastl::reference_wrapper<GraphicsEntityManager> m_entityManager;

        cppfs::FilePath m_filePath;

    public:
        explicit StaticMeshEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                  GraphicsModule& aGraphicsModule);

        [[nodiscard]]
        const cppfs::FilePath& GetFilePath() const;
        cppfs::FilePath& GetFilePath();

        void Load();
    };
}

SOGE_DI_REGISTER_NS(soge, StaticMeshEntity,
                    df::Factory<StaticMeshEntity, GraphicsCore, GeometryGraphicsPipeline, GraphicsModule>)

#endif // SOGE_GRAPHICS_ENTITIES_STATICMESHENTITY_HPP
