#include "sogepch.hpp"

#include "SOGE/Graphics/DirectionalLightEntity.hpp"

#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"


namespace soge
{
    DirectionalLightEntity::DirectionalLightEntity(GraphicsCore& aCore, DirectionalLightGraphicsPipeline& aPipeline,
                                                   const glm::vec3 aColor, const float aIntensity,
                                                   const glm::vec3 aDirection)
        : m_core{aCore}, m_pipeline{aPipeline}, m_color{aColor}, m_intensity{aIntensity}, m_direction{aDirection}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        SOGE_INFO_LOG("Creating NVRHI constant buffer for directional light entity...");
        nvrhi::BufferDesc constantBufferDesc{};
        constantBufferDesc.byteSize = sizeof(ConstantBuffer);
        constantBufferDesc.isConstantBuffer = true;
        constantBufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        constantBufferDesc.keepInitialState = true;
        constantBufferDesc.debugName = "SOGE directional light entity constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(constantBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set for directional light entity...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        bindingSetDesc.addItem(nvrhi::BindingSetItem::ConstantBuffer(1, m_nvrhiConstantBuffer));
        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, &aPipeline.GetEntityBindingLayout());

        SOGE_INFO_LOG("Updating NVRHI constant buffer for directional light entity...");
        const nvrhi::CommandListHandle updateCommandList = m_core.get().GetRawDevice().createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            const ConstantBuffer constantBuffer{
                .m_color = m_color,
                .m_intensity = m_intensity,
                .m_direction = m_direction,
            };
            commandList->writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    nvrhi::BindingSetHandle DirectionalLightEntity::GetBindingSet(Tag)
    {
        return m_nvrhiBindingSet;
    }

    nvrhi::BufferHandle DirectionalLightEntity::GetConstantBuffer(Tag)
    {
        return m_nvrhiConstantBuffer;
    }

    glm::vec3 DirectionalLightEntity::GetColor() const
    {
        return m_color;
    }

    void DirectionalLightEntity::SetColor(const glm::vec3 aColor)
    {
        if (m_color == aColor)
        {
            return;
        }
        m_color = aColor;

        SOGE_INFO_LOG("Updating NVRHI constant buffer for directional light entity...");
        const nvrhi::CommandListHandle updateCommandList = m_core.get().GetRawDevice().createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            const ConstantBuffer constantBuffer{
                .m_color = m_color,
                .m_intensity = m_intensity,
                .m_direction = m_direction,
            };
            commandList->writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    float DirectionalLightEntity::GetIntensity() const
    {
        return m_intensity;
    }

    void DirectionalLightEntity::SetIntensity(const float aIntensity)
    {
        if (m_intensity == aIntensity)
        {
            return;
        }
        m_intensity = aIntensity;

        SOGE_INFO_LOG("Updating NVRHI constant buffer for directional light entity...");
        const nvrhi::CommandListHandle updateCommandList = m_core.get().GetRawDevice().createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            const ConstantBuffer constantBuffer{
                .m_color = m_color,
                .m_intensity = m_intensity,
                .m_direction = m_direction,
            };
            commandList->writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    glm::vec3 DirectionalLightEntity::GetDirection() const
    {
        return m_direction;
    }

    void DirectionalLightEntity::SetDirection(const glm::vec3 aDirection)
    {
        if (m_direction == aDirection)
        {
            return;
        }
        m_direction = aDirection;

        SOGE_INFO_LOG("Updating NVRHI constant buffer for directional light entity...");
        const nvrhi::CommandListHandle updateCommandList = m_core.get().GetRawDevice().createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};

            const ConstantBuffer constantBuffer{
                .m_color = m_color,
                .m_intensity = m_intensity,
                .m_direction = m_direction,
            };
            commandList->writeBuffer(m_nvrhiConstantBuffer, &constantBuffer, sizeof(constantBuffer));
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }
}
