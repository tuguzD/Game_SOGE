#ifndef SOGE_GRAPHICS_POINTLIGHTENTITY_HPP
#define SOGE_GRAPHICS_POINTLIGHTENTITY_HPP

#include "SOGE/Graphics/Deferred/PointLightGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class PointLightEntity : public GraphicsEntity, public PointLightGraphicsPipeline::Entity
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<PointLightGraphicsPipeline> m_pipeline;

        ConstantBufferData m_constantBufferData;
        bool m_shouldWrite;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;
        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;

    public:
        using Attenuation = ConstantBufferData::Attenuation;

        explicit PointLightEntity(GraphicsCore& aCore, PointLightGraphicsPipeline& aPipeline,
                                  glm::vec3 aColor = glm::vec3{1.0f}, float aIntensity = 1.0f,
                                  glm::vec3 aPosition = glm::vec3{}, const Attenuation& aAttenuation = Attenuation{});

        [[nodiscard]]
        glm::vec3 GetColor() const;
        glm::vec3& GetColor();

        [[nodiscard]]
        float GetIntensity() const;
        float& GetIntensity();

        [[nodiscard]]
        glm::vec3 GetPosition() const;
        glm::vec3& GetPosition();

        [[nodiscard]]
        Attenuation GetAttenuation() const;
        Attenuation& GetAttenuation();

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetVertexBuffer(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetIndexBuffer(Tag) override;

        void WriteConstantBuffer(Tag, nvrhi::ICommandList& aCommandList) override;
        void WriteVertexBuffer(Tag, nvrhi::ICommandList& aCommandList) override;
        void WriteIndexBuffer(Tag, nvrhi::ICommandList& aCommandList) override;
    };
}

SOGE_DI_REGISTER_NS(soge, PointLightEntity, df::Factory<PointLightEntity, GraphicsCore, PointLightGraphicsPipeline>)

#endif // SOGE_GRAPHICS_POINTLIGHTENTITY_HPP
