#include "sogepch.hpp"

#include "SOGE/Graphics/AmbientLightEntity.hpp"

#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"


namespace soge
{
    AmbientLightEntity::AmbientLightEntity(GraphicsCore& aCore, AmbientLightGraphicsPipeline& aPipeline,
                                           const glm::vec3 aColor, const float aIntensity)
        : m_core{aCore}, m_pipeline{aPipeline}, m_color{aColor}, m_intensity{aIntensity}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        SOGE_INFO_LOG("Creating NVRHI constant buffer for ambient light entity...");
        nvrhi::BufferDesc constantBufferDesc{};
        constantBufferDesc.byteSize = sizeof(ConstantBuffer);
        constantBufferDesc.isConstantBuffer = true;
        constantBufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        constantBufferDesc.keepInitialState = true;
        constantBufferDesc.debugName = "SOGE ambient light entity constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(constantBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set for ambient light entity...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        bindingSetDesc.addItem(nvrhi::BindingSetItem::ConstantBuffer(0, m_nvrhiConstantBuffer));
        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, &aPipeline.GetEntityBindingLayout());

        SOGE_INFO_LOG("Updating NVRHI constant buffer for ambient light entity...");
        const nvrhi::CommandListHandle updateCommandList = m_core.get().GetRawDevice().createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            const ConstantBuffer constantBuffer{
                .m_color = m_color,
                .m_intensity = m_intensity,
            };
            commandList->writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    nvrhi::BindingSetHandle AmbientLightEntity::GetBindingSet(Tag)
    {
        return m_nvrhiBindingSet;
    }

    nvrhi::BufferHandle AmbientLightEntity::GetConstantBuffer(Tag)
    {
        return m_nvrhiConstantBuffer;
    }

    glm::vec3 AmbientLightEntity::GetColor() const
    {
        return m_color;
    }

    void AmbientLightEntity::SetColor(const glm::vec3 aColor)
    {
        if (m_color == aColor)
        {
            return;
        }
        m_color = aColor;

        SOGE_INFO_LOG("Updating NVRHI constant buffer for ambient light entity...");
        const nvrhi::CommandListHandle updateCommandList = m_core.get().GetRawDevice().createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            const ConstantBuffer constantBuffer{
                .m_color = m_color,
                .m_intensity = m_intensity,
            };
            commandList->writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    float AmbientLightEntity::GetIntensity() const
    {
        return m_intensity;
    }

    void AmbientLightEntity::SetIntensity(const float aIntensity)
    {
        if (m_intensity == aIntensity)
        {
            return;
        }
        m_intensity = aIntensity;

        SOGE_INFO_LOG("Updating NVRHI constant buffer for ambient light entity...");
        const nvrhi::CommandListHandle updateCommandList = m_core.get().GetRawDevice().createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            const ConstantBuffer constantBuffer{
                .m_color = m_color,
                .m_intensity = m_intensity,
            };
            commandList->writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }
}
