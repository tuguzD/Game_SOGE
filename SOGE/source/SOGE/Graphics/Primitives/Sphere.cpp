#include "sogepch.hpp"

#include "SOGE/Graphics/Primitives/Sphere.hpp"


namespace soge
{
    using Vertex = GeometryEntity::Vertex;
    using Index = GeometryEntity::Index;

    GeometryEntity CreateSphere(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline, const Transform& aTransform,
                                const GeometryEntity::Material& aMaterial, const uint32_t aMeridians,
                                const uint32_t aParallels, const float aRadius, const glm::vec3 aColor)
    {
        auto vertices = CreateSphereVertices(aMeridians, aParallels, aRadius, aColor);
        auto indices = CreateSphereIndices(aMeridians, aParallels);

        return GeometryEntity{
            aCore, aPipeline, aTransform, aMaterial, std::move(vertices), std::move(indices),
        };
    }

    eastl::vector<Vertex> CreateSphereVertices(const uint32_t aMeridians, const uint32_t aParallels,
                                               const float aRadius, const glm::vec3 aColor)
    {
        eastl::vector<Vertex> vertices;

        constexpr glm::vec3 firstNormal{0.0f, 1.0f, 0.0f};
        vertices.emplace_back(Vertex{firstNormal * aRadius, firstNormal, aColor});

        for (uint32_t j = 0; j < aParallels - 1; ++j)
        {
            constexpr float pi = glm::pi<float>();

            const float polar = pi * static_cast<float>(j + 1) / static_cast<float>(aParallels);
            const float sp = std::sin(polar);
            const float cp = std::cos(polar);

            for (uint32_t i = 0; i < aMeridians; ++i)
            {
                const float azimuth = 2.0f * pi * static_cast<float>(i) / static_cast<float>(aMeridians);
                const float sa = std::sin(azimuth);
                const float ca = std::cos(azimuth);
                const float x = sp * ca;
                const float y = cp;
                const float z = sp * sa;

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
            const Index a = i + 1;
            const Index b = (i + 1) % aMeridians + 1;

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
                const Index a = aStart + i;
                const Index a1 = aStart + (i + 1) % aMeridians;
                const Index b = bStart + i;
                const Index b1 = bStart + (i + 1) % aMeridians;

                indices.push_back(a);
                indices.push_back(a1);
                indices.push_back(b1);

                indices.push_back(a);
                indices.push_back(b1);
                indices.push_back(b);
            }
        }

        for (Index i = 0; i < aMeridians; ++i)
        {
            const Index a = i + aMeridians * (aParallels - 2) + 1;
            const Index b = (i + 1) % aMeridians + aMeridians * (aParallels - 2) + 1;

            indices.push_back((aParallels - 1) * aMeridians - 1);
            indices.push_back(a);
            indices.push_back(b);
        }

        return indices;
    }

    SpherePrimitive::SpherePrimitive(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                     const Transform& aTransform, const Material& aMaterial, const uint32_t aMeridians,
                                     const uint32_t aParallels, const float aRadius, const glm::vec3 aColor)
        : m_geometryEntity{aCore, aPipeline, aTransform, aMaterial}, m_meridians{aMeridians}, m_parallels{aParallels},
          m_radius{aRadius}, m_color{aColor}, m_shouldWrite{true}
    {
    }

    Transform SpherePrimitive::GetTransform() const
    {
        return m_geometryEntity.GetTransform();
    }

    Transform& SpherePrimitive::GetTransform()
    {
        return m_geometryEntity.GetTransform();
    }

    auto SpherePrimitive::GetMaterial() const -> Material
    {
        return m_geometryEntity.GetMaterial();
    }

    auto SpherePrimitive::GetMaterial() -> Material&
    {
        return m_geometryEntity.GetMaterial();
    }

    uint32_t SpherePrimitive::GetMeridians() const
    {
        return m_meridians;
    }

    uint32_t& SpherePrimitive::GetMeridians()
    {
        m_shouldWrite = true;
        return m_meridians;
    }

    uint32_t SpherePrimitive::GetParallels() const
    {
        return m_parallels;
    }

    uint32_t& SpherePrimitive::GetParallels()
    {
        m_shouldWrite = true;
        return m_parallels;
    }

    float SpherePrimitive::GetRadius() const
    {
        return m_radius;
    }

    float& SpherePrimitive::GetRadius()
    {
        m_shouldWrite = true;
        return m_radius;
    }

    glm::vec3 SpherePrimitive::GetColor() const
    {
        return m_color;
    }

    glm::vec3& SpherePrimitive::GetColor()
    {
        m_shouldWrite = true;
        return m_color;
    }

    eastl::span<const Vertex> SpherePrimitive::GetVertices() const
    {
        return m_geometryEntity.GetVertices();
    }

    eastl::span<const Index> SpherePrimitive::GetIndices() const
    {
        return m_geometryEntity.GetIndices();
    }

    nvrhi::BindingSetHandle SpherePrimitive::GetBindingSet(const Tag aTag)
    {
        return m_geometryEntity.GetBindingSet(aTag);
    }

    nvrhi::BufferHandle SpherePrimitive::GetVertexBuffer(const Tag aTag)
    {
        return m_geometryEntity.GetVertexBuffer(aTag);
    }

    nvrhi::BufferHandle SpherePrimitive::GetIndexBuffer(const Tag aTag)
    {
        return m_geometryEntity.GetIndexBuffer(aTag);
    }

    void SpherePrimitive::WriteResources(const Tag aTag, nvrhi::ICommandList& aCommandList)
    {
        if (m_shouldWrite)
        {
            m_shouldWrite = false;

            m_geometryEntity.GetVertices() = CreateSphereVertices(m_meridians, m_parallels, m_radius, m_color);
            m_geometryEntity.GetIndices() = CreateSphereIndices(m_meridians, m_parallels);
        }

        m_geometryEntity.WriteResources(aTag, aCommandList);
    }
}
