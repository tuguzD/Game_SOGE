#include "sogepch.hpp"

#include "SOGE/Graphics/AmbientLightEntity.hpp"


namespace soge
{
    AmbientLightEntity::AmbientLightEntity(GraphicsCore& aCore, AmbientLightGraphicsPipeline& aPipeline,
                                           const glm::vec3 aColor, const float aIntensity)
        : m_core{aCore}, m_pipeline{aPipeline}, m_constantBufferData{.m_color = aColor, .m_intensity = aIntensity},
          m_shouldWrite{true}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        SOGE_INFO_LOG("Creating NVRHI constant buffer for ambient light entity...");
        nvrhi::BufferDesc constantBufferDesc{};
        constantBufferDesc.byteSize = sizeof(ConstantBufferData);
        constantBufferDesc.isConstantBuffer = true;
        constantBufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        constantBufferDesc.keepInitialState = true;
        constantBufferDesc.debugName = "SOGE ambient light entity constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(constantBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set for ambient light entity...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        bindingSetDesc.bindings = {
            nvrhi::BindingSetItem::ConstantBuffer(0, m_nvrhiConstantBuffer),
        };
        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, &aPipeline.GetEntityBindingLayout());
    }

    glm::vec3 AmbientLightEntity::GetColor() const
    {
        return m_constantBufferData.m_color;
    }

    glm::vec3& AmbientLightEntity::GetColor()
    {
        m_shouldWrite = true;
        return m_constantBufferData.m_color;
    }

    float AmbientLightEntity::GetIntensity() const
    {
        return m_constantBufferData.m_intensity;
    }

    float& AmbientLightEntity::GetIntensity()
    {
        m_shouldWrite = true;
        return m_constantBufferData.m_intensity;
    }

    nvrhi::BindingSetHandle AmbientLightEntity::GetBindingSet(Tag)
    {
        return m_nvrhiBindingSet;
    }

    void AmbientLightEntity::WriteConstantBuffer(Tag, nvrhi::ICommandList& aCommandList)
    {
        if (!m_shouldWrite)
        {
            return;
        }
        m_shouldWrite = false;

        SOGE_INFO_LOG("Updating NVRHI constant buffer for ambient light entity...");
        aCommandList.writeBuffer(m_nvrhiConstantBuffer, &m_constantBufferData, sizeof(m_constantBufferData));
    }
}
