#include "sogepch.hpp"

#include "SOGE/Graphics/Primitives/Box.hpp"


namespace
{
    using Vertex = soge::GeometryEntity::Vertex;
    using Index = soge::GeometryEntity::Index;

    [[nodiscard]]
    eastl::vector<Vertex> CreateBoxVerticesVector(const glm::vec3 aDimensions = glm::vec3{1.0f},
                                                  const glm::vec3 aColor = glm::vec3{1.0f})
    {
        const auto vertices = soge::CreateBoxVertices(aDimensions, aColor);

        eastl::vector<Vertex> vector;
        vector.assign(vertices.begin(), vertices.end());
        return vector;
    }

    [[nodiscard]]
    eastl::vector<Index> CreateBoxIndicesVector()
    {
        constexpr auto indices = soge::CreateBoxIndices();

        eastl::vector<Index> vector;
        vector.assign(indices.begin(), indices.end());
        return vector;
    }
}

namespace soge
{
    GeometryEntity CreateBox(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline, const Transform& aTransform,
                             GeometryEntity::Material aMaterial, const glm::vec3 aDimensions, const glm::vec3 aColor)
    {
        auto vertices = CreateBoxVerticesVector(aDimensions, aColor);
        auto indices = CreateBoxIndicesVector();

        return GeometryEntity{aCore, aPipeline, aTransform, aMaterial, std::move(vertices), std::move(indices)};
    }

    BoxPrimitive::BoxPrimitive(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline, const Transform& aTransform,
                               const Material& aMaterial, const glm::vec3 aDimensions, const glm::vec3 aColor)
        : m_geometryEntity{aCore, aPipeline, aTransform, aMaterial}, m_dimensions{aDimensions}, m_color{aColor},
          m_shouldWrite{true}
    {
    }

    Transform BoxPrimitive::GetTransform() const
    {
        return m_geometryEntity.GetTransform();
    }

    Transform& BoxPrimitive::GetTransform()
    {
        return m_geometryEntity.GetTransform();
    }

    nvrhi::ITexture* BoxPrimitive::GetColorTexture() const
    {
        return m_geometryEntity.GetColorTexture();
    }

    nvrhi::TextureHandle& BoxPrimitive::GetColorTexture()
    {
        return m_geometryEntity.GetColorTexture();
    }

    auto BoxPrimitive::GetMaterial() const -> Material
    {
        return m_geometryEntity.GetMaterial();
    }

    auto BoxPrimitive::GetMaterial() -> Material&
    {
        return m_geometryEntity.GetMaterial();
    }

    glm::vec3 BoxPrimitive::GetDimensions() const
    {
        return m_dimensions;
    }

    glm::vec3& BoxPrimitive::GetDimensions()
    {
        m_shouldWrite = true;
        return m_dimensions;
    }

    glm::vec3 BoxPrimitive::GetColor() const
    {
        return m_color;
    }

    glm::vec3& BoxPrimitive::GetColor()
    {
        m_shouldWrite = true;
        return m_color;
    }

    eastl::span<const Vertex> BoxPrimitive::GetVertices() const
    {
        return m_geometryEntity.GetVertices();
    }

    eastl::span<const Index> BoxPrimitive::GetIndices() const
    {
        return m_geometryEntity.GetIndices();
    }

    nvrhi::BindingSetHandle BoxPrimitive::GetBindingSet(const Tag aTag)
    {
        return m_geometryEntity.GetBindingSet(aTag);
    }

    nvrhi::BufferHandle BoxPrimitive::GetVertexBuffer(const Tag aTag)
    {
        return m_geometryEntity.GetVertexBuffer(aTag);
    }

    nvrhi::BufferHandle BoxPrimitive::GetIndexBuffer(const Tag aTag)
    {
        return m_geometryEntity.GetIndexBuffer(aTag);
    }

    void BoxPrimitive::WriteResources(nvrhi::ICommandList& aCommandList)
    {
        if (m_shouldWrite)
        {
            m_shouldWrite = false;

            m_geometryEntity.GetVertices() = CreateBoxVerticesVector(m_dimensions, m_color);
            m_geometryEntity.GetIndices() = CreateBoxIndicesVector();
        }

        m_geometryEntity.WriteResources(aCommandList);
    }
}
