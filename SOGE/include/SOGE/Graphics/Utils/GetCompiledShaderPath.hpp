#ifndef SOGE_GRAPHICS_UTILS_GETCOMPILEDSHADERPATH_HPP
#define SOGE_GRAPHICS_UTILS_GETCOMPILEDSHADERPATH_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"

#include <cppfs/FilePath.h>


namespace soge
{
    [[nodiscard]]
    cppfs::FilePath GetCompiledShaderPath(const GraphicsCore& aCore, const cppfs::FilePath& aSourcePath,
                                          eastl::string_view aEntryName = "");
}

#endif // SOGE_GRAPHICS_UTILS_GETCOMPILEDSHADERPATH_HPP
