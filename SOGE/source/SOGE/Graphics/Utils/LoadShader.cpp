#include "sogepch.hpp"

#include "SOGE/Graphics/Utils/LoadShader.hpp"

#include "SOGE/Graphics/Utils/GetCompiledShaderPath.hpp"

#include <cppfs/FileHandle.h>
#include <cppfs/fs.h>


namespace soge
{
    nvrhi::ShaderHandle LoadShader(nvrhi::IDevice& aDevice, const nvrhi::ShaderDesc& aDesc,
                                   const eastl::span<const std::uint8_t> aShaderBinary)
    {
        return aDevice.createShader(aDesc, aShaderBinary.data(), aShaderBinary.size());
    }

    nvrhi::ShaderHandle LoadShader(GraphicsCore& aCore, const nvrhi::ShaderDesc& aDesc,
                                   const cppfs::FilePath& aSourcePath, const eastl::string_view aEntryName)
    {
        const auto compiledPath = soge::GetCompiledShaderPath(aCore, aSourcePath, aEntryName);
        const auto shaderFile = cppfs::fs::open(compiledPath.path());
        if (!shaderFile.exists())
        {
            const auto errorMessage = fmt::format(R"(Shader file "{}" does not exist)", compiledPath.path());
            throw std::runtime_error{errorMessage};
        }

        const auto stream = shaderFile.createInputStream(std::ios::in | std::ios::binary);
        const std::vector<std::uint8_t> shaderBinary{std::istreambuf_iterator{*stream}, {}};
        return LoadShader(aCore.GetRawDevice(), aDesc, shaderBinary);
    }
}
