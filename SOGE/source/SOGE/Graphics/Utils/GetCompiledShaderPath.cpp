#include "sogepch.hpp"

#include "SOGE/Graphics/Utils/GetCompiledShaderPath.hpp"


namespace soge
{
    cppfs::FilePath GetCompiledShaderPath(const GraphicsCore& aCore, const cppfs::FilePath& aSourcePath,
                                          const eastl::string_view aEntryName)
    {
        cppfs::FilePath destinationPath{aSourcePath};

        if (!aEntryName.empty())
        {
            destinationPath = fmt::format("{}{}_{}{}", destinationPath.directoryPath(), destinationPath.baseName(),
                                          aEntryName.data(), destinationPath.extension());
        }

        const auto extension = aCore.GetCompiledShaderExtension();
        destinationPath =
            fmt::format("{}{}.{}", destinationPath.directoryPath(), destinationPath.baseName(), extension.data());

        return destinationPath;
    }
}
