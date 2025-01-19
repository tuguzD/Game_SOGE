#include "sogepch.hpp"

#include "SOGE/Graphics/Primitives/Sphere.hpp"


namespace soge
{
    using Vertex = GeometryEntity::Vertex;
    using Index = GeometryEntity::Index;

    GeometryEntity CreateSphere(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                const CreateSphereParams& aParams)
    {
        auto vertices =
            CreateSphereVertices(aParams.m_color, aParams.m_meridians, aParams.m_parallels, aParams.m_radius);
        auto indices = CreateSphereIndices(aParams.m_meridians, aParams.m_parallels);

        return GeometryEntity{
            aCore, aPipeline, aParams.m_transform, std::move(vertices), std::move(indices),
        };
    }

    eastl::vector<Vertex> CreateSphereVertices(const glm::vec3 aColor, const uint32_t aMeridians,
                                               const uint32_t aParallels, const float aRadius)
    {
        eastl::vector<Vertex> vertices;

        constexpr glm::vec3 firstNormal{0.0f, 1.0f, 0.0f};
        vertices.emplace_back(Vertex{firstNormal * aRadius, firstNormal, aColor});

        for (uint32_t j = 0; j < aParallels - 1; ++j)
        {
            constexpr float pi = glm::pi<float>();

            const double polar = pi * static_cast<double>(j + 1) / static_cast<double>(aParallels);
            const double sp = std::sin(polar);
            const double cp = std::cos(polar);

            for (uint32_t i = 0; i < aMeridians; ++i)
            {
                const double azimuth = 2.0 * pi * static_cast<double>(i) / static_cast<double>(aMeridians);
                const double sa = std::sin(azimuth);
                const double ca = std::cos(azimuth);
                const double x = sp * ca;
                const double y = cp;
                const double z = sp * sa;

                const glm::vec3 normal{x, y, z};
                vertices.emplace_back(Vertex{normal * aRadius, normal, aColor});
            }
        }

        constexpr glm::vec3 lastNormal{0.0f, -1.0f, 0.0f};
        vertices.emplace_back(Vertex{lastNormal * aRadius, lastNormal, aColor});

        return vertices;
    }

    eastl::vector<Index> CreateSphereIndices(const uint32_t aMeridians, const uint32_t aParallels)
    {
        eastl::vector<Index> indices;

        for (uint32_t i = 0; i < aMeridians; ++i)
        {
            const uint32_t a = i + 1;
            const uint32_t b = (i + 1) % aMeridians + 1;

            indices.push_back(0);
            indices.push_back(b);
            indices.push_back(a);
        }

        for (uint32_t j = 0; j < aParallels - 2; ++j)
        {
            const uint32_t aStart = j * aMeridians + 1;
            const uint32_t bStart = (j + 1) * aMeridians + 1;
            for (uint32_t i = 0; i < aMeridians; ++i)
            {
                const uint32_t a = aStart + i;
                const uint32_t a1 = aStart + (i + 1) % aMeridians;
                const uint32_t b = bStart + i;
                const uint32_t b1 = bStart + (i + 1) % aMeridians;

                indices.push_back(a);
                indices.push_back(a1);
                indices.push_back(b1);

                indices.push_back(a);
                indices.push_back(b1);
                indices.push_back(b);
            }
        }

        for (uint32_t i = 0; i < aMeridians; ++i)
        {
            const uint32_t a = i + aMeridians * (aParallels - 2) + 1;
            const uint32_t b = (i + 1) % aMeridians + aMeridians * (aParallels - 2) + 1;

            indices.push_back((aParallels - 1) * aMeridians - 1);
            indices.push_back(a);
            indices.push_back(b);
        }

        return indices;
    }
}
