#ifndef SOGE_GRAPHICS_AMBIENTLIGHTENTITY_HPP
#define SOGE_GRAPHICS_AMBIENTLIGHTENTITY_HPP

#include "SOGE/Graphics/Deferred/AmbientLightGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class AmbientLightEntity : public GraphicsEntity, public AmbientLightGraphicsPipelineEntity
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<AmbientLightGraphicsPipeline> m_pipeline;

        glm::vec3 m_color;
        float m_intensity;

        nvrhi::BufferHandle m_nvrhiConstantBuffer;
        nvrhi::BindingSetHandle m_nvrhiBindingSet;

    public:
        explicit AmbientLightEntity(GraphicsCore& aCore, AmbientLightGraphicsPipeline& aPipeline,
                                    glm::vec3 aColor = glm::vec3{1.0f}, float aIntensity = 1.0f);

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
    };
}

SOGE_DI_REGISTER_NS(soge, AmbientLightEntity,
                    df::Factory<AmbientLightEntity, GraphicsCore, AmbientLightGraphicsPipeline>)

#endif // SOGE_GRAPHICS_AMBIENTLIGHTENTITY_HPP
