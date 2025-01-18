#ifndef SOGE_GRAPHICS_DIRECTIONALLIGHTENTITY_HPP
#define SOGE_GRAPHICS_DIRECTIONALLIGHTENTITY_HPP

#include "SOGE/Graphics/Deferred/DirectionalLightGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class DirectionalLightEntity : public GraphicsEntity, public DirectionalLightGraphicsPipeline::Entity
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<DirectionalLightGraphicsPipeline> m_pipeline;

        glm::vec3 m_color;
        float m_intensity;
        glm::vec3 m_direction;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;

    public:
        explicit DirectionalLightEntity(GraphicsCore& aCore, DirectionalLightGraphicsPipeline& aPipeline,
                                        glm::vec3 aColor = glm::vec3{1.0f}, float aIntensity = 1.0f,
                                        glm::vec3 aDirection = glm::vec3{0.0f, -1.0f, 0.0f});

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetConstantBuffer(Tag) override;

        [[nodiscard]]
        glm::vec3 GetColor() const;
        void SetColor(glm::vec3 aColor);

        [[nodiscard]]
        float GetIntensity() const;
        void SetIntensity(float aIntensity);

        [[nodiscard]]
        glm::vec3 GetDirection() const;
        void SetDirection(glm::vec3 aDirection);
    };
}

SOGE_DI_REGISTER_NS(soge, DirectionalLightEntity,
                    df::Factory<DirectionalLightEntity, GraphicsCore, DirectionalLightGraphicsPipeline>)

#endif // SOGE_GRAPHICS_DIRECTIONALLIGHTENTITY_HPP
