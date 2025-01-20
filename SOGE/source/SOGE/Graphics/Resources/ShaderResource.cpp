#include "sogepch.hpp"

#include "SOGE/Graphics/Resources/ShaderResource.hpp"

#include "SOGE/Graphics/Utils/LoadShader.hpp"


namespace soge
{
    ShaderResource::ShaderResource(GraphicsCore& aCore, const eastl::string_view aName,
                                   const cppfs::FilePath& aFullPath, const nvrhi::ShaderDesc& aShaderDesc)
        : GraphicsResource(aName, aFullPath)
    {
        m_shaderHandle = LoadShader(aCore, aShaderDesc, aFullPath);
    }

    nvrhi::IShader& ShaderResource::GetResource()
    {
        return *m_shaderHandle;
    }
}
