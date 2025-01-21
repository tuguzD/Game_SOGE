#ifndef SOGE_GRAPHICS_UTILS_LOADSHADER_HPP
#define SOGE_GRAPHICS_UTILS_LOADSHADER_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"

#include <cppfs/FilePath.h>


namespace soge
{
    [[nodiscard]]
    nvrhi::ShaderHandle LoadShader(nvrhi::IDevice& aDevice, const nvrhi::ShaderDesc& aDesc,
                                   eastl::span<const std::uint8_t> aShaderBinary);

    [[nodiscard]]
    nvrhi::ShaderHandle LoadShader(GraphicsCore& aCore, const nvrhi::ShaderDesc& aDesc,
                                   const cppfs::FilePath& aSourcePath);
}

#endif // SOGE_GRAPHICS_UTILS_LOADSHADER_HPP
