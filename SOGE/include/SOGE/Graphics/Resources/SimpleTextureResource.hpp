#ifndef SOGE_GRAPHICS_RESOURCES_SIMPLETEXTURERESOURCE_HPP
#define SOGE_GRAPHICS_RESOURCES_SIMPLETEXTURERESOURCE_HPP

#include "SOGE/Graphics/Resources/TextureResource.hpp"


namespace soge
{
    class SimpleTextureResource : public TextureResource
    {
    private:
        eastl::vector<nvrhi::Color> m_pixels;
        bool m_shouldWrite;

        void Initialize();

    public:
        explicit SimpleTextureResource(GraphicsCore& aCore, eastl::string_view aName, cppfs::FilePath aFullPath);

        [[nodiscard]]
        eastl::span<const nvrhi::Color> GetPixels() const noexcept;

        void WriteResource(nvrhi::ICommandList& aCommandList) override;

        bool Reload() override;
        void Unload() override;
        void Destroy() override;
    };
}

#endif // SOGE_GRAPHICS_RESOURCES_SIMPLETEXTURERESOURCE_HPP
