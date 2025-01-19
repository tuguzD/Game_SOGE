#ifndef SOGE_GRAPHICS_PRIMITIVES_SPHERE_HPP
#define SOGE_GRAPHICS_PRIMITIVES_SPHERE_HPP

#include "SOGE/Graphics/Entities/GeometryEntity.hpp"


namespace soge
{
    struct CreateSphereParams
    {
        Transform m_transform;
        glm::vec3 m_color{1.0f};
        uint32_t m_meridians = 32;
        uint32_t m_parallels = 16;
        float m_radius = 1.0f;
    };

    [[nodiscard]]
    GeometryEntity CreateSphere(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                const CreateSphereParams& aParams = CreateSphereParams{});

    [[nodiscard]]
    eastl::vector<GeometryEntity::Vertex> CreateSphereVertices(glm::vec3 aColor = glm::vec3{1.0f},
                                                               uint32_t aMeridians = 32, uint32_t aParallels = 16,
                                                               float aRadius = 1.0f);

    [[nodiscard]]
    eastl::vector<GeometryEntity::Index> CreateSphereIndices(uint32_t aMeridians = 32, uint32_t aParallels = 16);
}

#endif // SOGE_GRAPHICS_PRIMITIVES_SPHERE_HPP
