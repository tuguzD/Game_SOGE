#ifndef SOGE_GRAPHICS_ENTITIES_AMBIENTLIGHTENTITY_HPP
#define SOGE_GRAPHICS_ENTITIES_AMBIENTLIGHTENTITY_HPP

#include "SOGE/Graphics/Deferred/AmbientLightGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class AmbientLightEntity : public GraphicsEntity, public AmbientLightGraphicsPipeline::Entity
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<AmbientLightGraphicsPipeline> m_pipeline;

        ConstantBufferData m_constantBufferData;
        bool m_shouldWrite;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;

    public:
        explicit AmbientLightEntity(GraphicsCore& aCore, AmbientLightGraphicsPipeline& aPipeline,
                                    glm::vec3 aColor = glm::vec3{1.0f}, float aIntensity = 1.0f);

        [[nodiscard]]
        glm::vec3 GetColor() const;
        glm::vec3& GetColor();

        [[nodiscard]]
        float GetIntensity() const;
        float& GetIntensity();

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet(Tag) override;

        void WriteConstantBuffer(Tag, nvrhi::ICommandList& aCommandList) override;
    };
}

SOGE_DI_REGISTER_NS(soge, AmbientLightEntity,
                    df::Factory<AmbientLightEntity, GraphicsCore, AmbientLightGraphicsPipeline>)

#endif // SOGE_GRAPHICS_ENTITIES_AMBIENTLIGHTENTITY_HPP
