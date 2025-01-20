#include "sogepch.hpp"

#include "SOGE/Graphics/Entities/StaticMeshEntity.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>


namespace
{
    [[nodiscard]]
    soge::Transform TransformFromNode(const aiNode& aNode)
    {
        aiVector3D position, scale;
        aiQuaternion rotation;
        aNode.mTransformation.Decompose(scale, rotation, position);

        return soge::Transform{
            .m_position = glm::vec3{position.x, position.y, position.z},
            .m_rotation = glm::quat{rotation.x, rotation.y, rotation.z, rotation.w},
            .m_scale = glm::vec3{scale.x, scale.y, scale.z},
        };
    }

    using Vertex = soge::GeometryEntity::Vertex;
    using Index = soge::GeometryEntity::Index;

    [[nodiscard]]
    soge::GeometryEntity CreateEntityFromMesh(const aiScene& aScene, const aiMesh& aMesh, soge::GraphicsCore& aCore,
                                              soge::GeometryGraphicsPipeline& aPipeline)
    {
        // TODO: store material data
        if (const aiMaterial* aiMaterial = aScene.mNumMaterials > 0 ? aScene.mMaterials[aMesh.mMaterialIndex] : nullptr)
        {
        }

        eastl::vector<Vertex> vertices;
        vertices.reserve(aMesh.mNumVertices);
        for (std::uint32_t index = 0; index < aMesh.mNumVertices; ++index)
        {
            const auto [x, y, z] = aMesh.mVertices[index];
            const glm::vec3 position{x, y, z};

            glm::vec3 normal{};
            if (const aiVector3D* aiNormals = aMesh.mNormals)
            {
                const auto [nx, ny, nz] = aiNormals[index];
                normal = glm::vec3{nx, ny, nz};
            }

            glm::vec3 color{1.0f};
            if (const aiColor4D* colors = aMesh.mColors[0])
            {
                const auto [r, g, b, a] = colors[index];
                color *= glm::vec3{r, g, b};
            }

            vertices.emplace_back(Vertex{
                .m_position = position,
                .m_normal = normal,
                .m_color = color,
            });
        }

        eastl::vector<Index> indices;
        indices.reserve(static_cast<std::size_t>(aMesh.mNumFaces) * 3u);
        for (const aiFace& face : eastl::span{aMesh.mFaces, aMesh.mNumFaces})
        {
            for (const eastl::span aiIndices{face.mIndices, face.mNumIndices};
                 const std::uint32_t index : aiIndices | std::views::take(3))
            {
                indices.emplace_back(index);
            }
        }

        return soge::GeometryEntity{aCore, aPipeline, soge::Transform{}, vertices, indices};
    }

    void TraverseNode(const aiScene& aScene, const aiNode& aNode, soge::GraphicsCore& aCore,
                      soge::GeometryGraphicsPipeline& aPipeline, soge::GraphicsEntityManager& aEntityManager)
    {
        const auto nodeTransform = TransformFromNode(aNode);
        for (const std::size_t meshIndex : eastl::span{aNode.mMeshes, aNode.mNumMeshes})
        {
            const aiMesh* mesh = aScene.mMeshes[meshIndex];
            if (mesh == nullptr)
            {
                continue;
            }

            const auto [entity, entityUuid] = aEntityManager.CreateEntity<soge::GeometryEntity>(
                CreateEntityFromMesh(aScene, *mesh, aCore, aPipeline));
            SOGE_INFO_LOG("Created static mesh (parent node is {}) with UUID: {}", 0, entityUuid.str());
            // TODO: add entity into some container with transform hierarchy
        }

        for (const aiNode* childNode : eastl::span{aNode.mChildren, aNode.mNumChildren})
        {
            TraverseNode(aScene, *childNode, aCore, aPipeline, aEntityManager);
        }
    }
}

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

        constexpr std::uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
        const aiScene* scene = importer.ReadFile(m_filePath.path(), flags);
        if (scene == nullptr)
        {
            const char* message = importer.GetErrorString();
            throw std::runtime_error{message};
        }

        const aiNode* node = scene->mRootNode;
        if (node == nullptr)
        {
            return;
        }
        TraverseNode(*scene, *node, m_core, m_pipeline, m_entityManager);
    }
}
