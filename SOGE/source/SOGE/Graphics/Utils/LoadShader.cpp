#include "sogepch.hpp"

#include "SOGE/Graphics/Utils/LoadShader.hpp"

#include "SOGE/Graphics/Utils/GetCompiledShaderPath.hpp"

#include <fstream>


namespace soge
{
    nvrhi::ShaderHandle LoadShader(nvrhi::IDevice& aDevice, const nvrhi::ShaderDesc& aDesc,
                                   const eastl::span<const std::uint8_t> aShaderBinary)
    {
        return aDevice.createShader(aDesc, aShaderBinary.data(), aShaderBinary.size());
    }

    nvrhi::ShaderHandle LoadShader(GraphicsCore& aCore, const nvrhi::ShaderDesc& aDesc,
                                   const std::filesystem::path& aSourcePath, const eastl::string_view aEntryName)
    {
        const auto compiledPath = soge::GetCompiledShaderPath(aCore, aSourcePath, aEntryName);
        if (!std::filesystem::exists(compiledPath))
        {
            const auto errorMessage = fmt::format(R"(Shader file "{}" does not exist)", compiledPath.generic_string());
            throw std::runtime_error{errorMessage};
        }

        std::ifstream shaderFile{compiledPath, std::ios::in | std::ios::binary};
        const std::vector<std::uint8_t> shaderBinary{std::istreambuf_iterator{shaderFile}, {}};
        return LoadShader(aCore.GetRawDevice(), aDesc, shaderBinary);
    }
}
