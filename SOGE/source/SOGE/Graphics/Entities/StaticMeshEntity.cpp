#include "sogepch.hpp"

#include "SOGE/Graphics/Entities/StaticMeshEntity.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>


namespace soge
{
    StaticMeshEntity::StaticMeshEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                       GraphicsModule& aGraphicsModule)
        : m_core{aCore}, m_pipeline{aPipeline}, m_entityManager{aGraphicsModule.GetEntityManager()}
    {
    }

    const cppfs::FilePath& StaticMeshEntity::GetFilePath() const
    {
        return m_filePath;
    }

    cppfs::FilePath& StaticMeshEntity::GetFilePath()
    {
        return m_filePath;
    }

    void StaticMeshEntity::Load()
    {
        Assimp::Importer importer;

        constexpr std::uint32_t flags =
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_ConvertToLeftHanded;
        const aiScene* scene = importer.ReadFile(m_filePath.path(), flags);
        if (scene == nullptr)
        {
            const char* message = importer.GetErrorString();
            throw std::runtime_error{message};
        }

        if (const aiNode* node = scene->mRootNode)
        {
            SOGE_INFO_LOG("Found root node in static mesh {}", m_filePath.path());
        }
        else
        {
            SOGE_WARN_LOG("No root node found in static mesh {}", m_filePath.path());
        }
    }
}
