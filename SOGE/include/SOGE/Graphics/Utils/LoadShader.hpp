#ifndef SOGE_GRAPHICS_UTILS_LOADSHADER_HPP
#define SOGE_GRAPHICS_UTILS_LOADSHADER_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"

#include <EASTL/span.h>
#include <cppfs/FilePath.h>
#include <nvrhi/nvrhi.h>


namespace soge
{
    [[nodiscard]]
    nvrhi::ShaderHandle LoadShader(nvrhi::IDevice& aDevice, const nvrhi::ShaderDesc& aDesc,
                                   eastl::span<const std::uint8_t> aShaderBinary);

    [[nodiscard]]
    nvrhi::ShaderHandle LoadShader(GraphicsCore& aCore, const nvrhi::ShaderDesc& aDesc,
                                   const cppfs::FilePath& aSourcePath, eastl::string_view aEntryName = "");
}

#endif // SOGE_GRAPHICS_UTILS_LOADSHADER_HPP
