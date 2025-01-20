#include "sogepch.hpp"

#include "SOGE/Graphics/Resources/ShaderResource.hpp"

#include "SOGE/Graphics/Utils/LoadShader.hpp"


namespace soge
{
    ShaderResource::ShaderResource(GraphicsCore& aCore, const eastl::string_view aName, cppfs::FilePath aFullPath,
                                   nvrhi::ShaderDesc aShaderDesc)
        : GraphicsResource{aName, std::move(aFullPath)}, m_core{aCore}, m_shaderDesc{std::move(aShaderDesc)}
    {
        Initialize();
    }

    void ShaderResource::Initialize()
    {
        m_shaderHandle = LoadShader(m_core.get(), m_shaderDesc, GetFullPath().data());
        SetLoaded(m_shaderHandle != nullptr);
    }

    nvrhi::IShader* ShaderResource::GetResource()
    {
        return m_shaderHandle;
    }

    void ShaderResource::WriteResource(nvrhi::ICommandList& aCommandList)
    {
        // do nothing for shaders
    }

    bool ShaderResource::Reload()
    {
        if (m_shaderHandle == nullptr)
        {
            Initialize();
            return IsLoaded();
        }
        return true;
    }

    void ShaderResource::Unload()
    {
        m_shaderHandle = nullptr;
        SetLoaded(false);
    }

    void ShaderResource::Destroy()
    {
        m_shaderHandle = nullptr;
        SetLoaded(false);
    }
}
