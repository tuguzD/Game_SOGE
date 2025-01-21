#include "sogepch.hpp"

#include "SOGE/Graphics/Entities/DirectionalLightEntity.hpp"

#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"


namespace soge
{
    DirectionalLightEntity::DirectionalLightEntity(GraphicsCore& aCore, DirectionalLightGraphicsPipeline& aPipeline,
                                                   const glm::vec3 aColor, const float aIntensity,
                                                   const glm::vec3 aDirection)
        : m_core{aCore}, m_pipeline{aPipeline},
          m_constantBufferData{.m_color = aColor, .m_intensity = aIntensity, .m_direction = aDirection},
          m_shouldWrite{true}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        SOGE_INFO_LOG("Creating NVRHI constant buffer for directional light entity...");
        nvrhi::BufferDesc constantBufferDesc{};
        constantBufferDesc.byteSize = sizeof(ConstantBufferData);
        constantBufferDesc.isConstantBuffer = true;
        constantBufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        constantBufferDesc.keepInitialState = true;
        constantBufferDesc.debugName = "SOGE directional light entity constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(constantBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set for directional light entity...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        bindingSetDesc.bindings = {
            nvrhi::BindingSetItem::ConstantBuffer(1, m_nvrhiConstantBuffer),
        };
        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, &aPipeline.GetEntityBindingLayout());
    }

    glm::vec3 DirectionalLightEntity::GetColor() const
    {
        return m_constantBufferData.m_color;
    }

    glm::vec3& DirectionalLightEntity::GetColor()
    {
        m_shouldWrite = true;
        return m_constantBufferData.m_color;
    }

    float DirectionalLightEntity::GetIntensity() const
    {
        return m_constantBufferData.m_intensity;
    }

    float& DirectionalLightEntity::GetIntensity()
    {
        m_shouldWrite = true;
        return m_constantBufferData.m_intensity;
    }

    glm::vec3 DirectionalLightEntity::GetDirection() const
    {
        return m_constantBufferData.m_direction;
    }

    glm::vec3& DirectionalLightEntity::GetDirection()
    {
        m_shouldWrite = true;
        return m_constantBufferData.m_direction;
    }

    nvrhi::BindingSetHandle DirectionalLightEntity::GetBindingSet(Tag)
    {
        return m_nvrhiBindingSet;
    }

    void DirectionalLightEntity::WriteResources(nvrhi::ICommandList& aCommandList)
    {
        if (!m_shouldWrite)
        {
            return;
        }
        m_shouldWrite = false;

        SOGE_INFO_LOG("Updating NVRHI constant buffer for directional light entity...");
        aCommandList.writeBuffer(m_nvrhiConstantBuffer, &m_constantBufferData, sizeof(m_constantBufferData));
    }
}
