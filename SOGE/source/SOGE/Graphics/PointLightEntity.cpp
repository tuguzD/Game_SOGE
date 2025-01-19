#include "sogepch.hpp"

#include "SOGE/Graphics/PointLightEntity.hpp"

#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"


namespace
{
    using Attenuation = soge::PointLightEntity::Attenuation;

    // https://learnopengl.com/Advanced-Lighting/Deferred-Shading
    float CalculateRadius(const Attenuation& aAttenuation, const glm::vec3 aColor)
    {
        const auto linearF = aAttenuation.m_linearFactor;
        const auto quadF = aAttenuation.m_quadraticFactor;
        const auto constF = aAttenuation.m_constFactor;

        const auto radiusAll =
            (-linearF + glm::sqrt(linearF * linearF - 4.0f * quadF * (constF - (256.0f / 5.0f) * aColor))) /
            (2.0f * quadF);
        return glm::max(radiusAll.x, radiusAll.y, radiusAll.z);
    }

    // https://github.com/caosdoar/spheres/blob/master/src/spheres.cpp#L262
    using Vertex = soge::PointLightEntity::Vertex;
    using Index = soge::PointLightEntity::Index;
    std::pair<std::vector<Vertex>, std::vector<Index>> UvSphere(const uint32_t aMeridians = 32,
                                                                const uint32_t aParallels = 10,
                                                                const float aRadius = 1.0f)
    {
        std::vector<Vertex> vertices;
        vertices.emplace_back(glm::vec3{0.0f, 1.0f, 0.0f} * aRadius);
        for (uint32_t j = 0; j < aParallels - 1; ++j)
        {
            double const polar = glm::pi<float>() * static_cast<double>(j + 1) / static_cast<double>(aParallels);
            double const sp = std::sin(polar);
            double const cp = std::cos(polar);
            for (uint32_t i = 0; i < aMeridians; ++i)
            {
                double const azimuth =
                    2.0 * glm::pi<float>() * static_cast<double>(i) / static_cast<double>(aMeridians);
                double const sa = std::sin(azimuth);
                double const ca = std::cos(azimuth);
                double const x = sp * ca;
                double const y = cp;
                double const z = sp * sa;
                vertices.emplace_back(glm::vec3{x, y, z} * aRadius);
            }
        }
        vertices.emplace_back(glm::vec3{0.0f, -1.0f, 0.0f} * aRadius);

        std::vector<Index> indices;
        for (uint32_t i = 0; i < aMeridians; ++i)
        {
            uint32_t const a = i + 1;
            uint32_t const b = (i + 1) % aMeridians + 1;

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

            indices.push_back(static_cast<uint32_t>(vertices.size()) - 1);
            indices.push_back(a);
            indices.push_back(b);
        }

        return {vertices, indices};
    }
}

namespace soge
{
    PointLightEntity::PointLightEntity(GraphicsCore& aCore, PointLightGraphicsPipeline& aPipeline,
                                       const glm::vec3 aColor, const float aIntensity, glm::vec3 aPosition,
                                       const Attenuation& aAttenuation)
        : m_core{aCore}, m_pipeline{aPipeline},
          m_constantBufferData{
              .m_model = glm::mat4x4{1.0f},
              .m_color = aColor,
              .m_intensity = aIntensity,
              .m_position = aPosition,
              .m_attenuation = aAttenuation,
          },
          m_shouldWrite{true}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        SOGE_INFO_LOG("Creating NVRHI constant buffer for point light entity...");
        nvrhi::BufferDesc constantBufferDesc{};
        constantBufferDesc.byteSize = sizeof(ConstantBufferData);
        constantBufferDesc.isConstantBuffer = true;
        constantBufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        constantBufferDesc.keepInitialState = true;
        constantBufferDesc.debugName = "SOGE point light entity constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(constantBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set for point light entity...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        bindingSetDesc.bindings = {
            nvrhi::BindingSetItem::ConstantBuffer(1, m_nvrhiConstantBuffer),
        };
        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, &aPipeline.GetEntityBindingLayout());

        const auto [vertices, indices] = UvSphere();
        eastl::span verticesSpan{vertices};
        eastl::span indicesSpan{indices};

        SOGE_INFO_LOG("Creating NVRHI vertex buffer for point light entity...");
        nvrhi::BufferDesc vertexBufferDesc{};
        vertexBufferDesc.byteSize = verticesSpan.size_bytes();
        vertexBufferDesc.isVertexBuffer = true;
        vertexBufferDesc.initialState = nvrhi::ResourceStates::VertexBuffer;
        vertexBufferDesc.keepInitialState = true;
        vertexBufferDesc.debugName = "SOGE point light entity vertex buffer";
        m_nvrhiVertexBuffer = device.createBuffer(vertexBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI index buffer for point light entity...");
        nvrhi::BufferDesc indexBufferDesc{};
        indexBufferDesc.byteSize = indicesSpan.size_bytes();
        indexBufferDesc.isIndexBuffer = true;
        indexBufferDesc.initialState = nvrhi::ResourceStates::IndexBuffer;
        indexBufferDesc.keepInitialState = true;
        indexBufferDesc.debugName = "SOGE point light entity index buffer";
        m_nvrhiIndexBuffer = device.createBuffer(indexBufferDesc);

        SOGE_INFO_LOG("Updating NVRHI vertex & index buffer for point light entity...");
        const nvrhi::CommandListHandle updateCommandList = device.createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            commandList->writeBuffer(m_nvrhiVertexBuffer, verticesSpan.data(), verticesSpan.size_bytes());
            commandList->writeBuffer(m_nvrhiIndexBuffer, indicesSpan.data(), indicesSpan.size_bytes());
        }
        aCore.ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    glm::vec3 PointLightEntity::GetColor() const
    {
        return m_constantBufferData.m_color;
    }

    glm::vec3& PointLightEntity::GetColor()
    {
        m_shouldWrite = true;
        return m_constantBufferData.m_color;
    }

    float PointLightEntity::GetIntensity() const
    {
        return m_constantBufferData.m_intensity;
    }

    float& PointLightEntity::GetIntensity()
    {
        m_shouldWrite = true;
        return m_constantBufferData.m_intensity;
    }

    glm::vec3 PointLightEntity::GetPosition() const
    {
        return m_constantBufferData.m_position;
    }

    glm::vec3& PointLightEntity::GetPosition()
    {
        m_shouldWrite = true;
        return m_constantBufferData.m_position;
    }

    auto PointLightEntity::GetAttenuation() const -> Attenuation
    {
        return m_constantBufferData.m_attenuation;
    }

    auto PointLightEntity::GetAttenuation() -> Attenuation&
    {
        m_shouldWrite = true;
        return m_constantBufferData.m_attenuation;
    }

    nvrhi::BindingSetHandle PointLightEntity::GetBindingSet(Tag)
    {
        return m_nvrhiBindingSet;
    }

    nvrhi::BufferHandle PointLightEntity::GetVertexBuffer(Tag)
    {
        return m_nvrhiVertexBuffer;
    }

    nvrhi::BufferHandle PointLightEntity::GetIndexBuffer(Tag)
    {
        return m_nvrhiIndexBuffer;
    }

    void PointLightEntity::WriteConstantBuffer(Tag, nvrhi::ICommandList& aCommandList)
    {
        if (!m_shouldWrite)
        {
            return;
        }
        m_shouldWrite = false;

        SOGE_INFO_LOG("Updating NVRHI constant buffer for point light entity...");
        const float radius = CalculateRadius(m_constantBufferData.m_attenuation,
                                             m_constantBufferData.m_color * m_constantBufferData.m_intensity);
        SOGE_INFO_LOG("Point light sphere radius is {}", radius);
        const Transform transform{
            .m_position = m_constantBufferData.m_position,
            .m_scale = glm::vec3{radius},
        };
        m_constantBufferData.m_model = transform.WorldMatrix();

        aCommandList.writeBuffer(m_nvrhiConstantBuffer, &m_constantBufferData, sizeof(m_constantBufferData));
    }

    void PointLightEntity::WriteVertexBuffer(Tag, nvrhi::ICommandList& aCommandList)
    {
    }

    void PointLightEntity::WriteIndexBuffer(Tag, nvrhi::ICommandList& aCommandList)
    {
    }
}
