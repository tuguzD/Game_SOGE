#ifndef SOGE_GRAPHICS_RESOURCES_TEXTURERESOURCE_HPP
#define SOGE_GRAPHICS_RESOURCES_TEXTURERESOURCE_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/GraphicsResource.hpp"


namespace soge
{
    class TextureResource : public GraphicsResource
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;

        nvrhi::TextureHandle m_textureHandle;
        nvrhi::TextureDesc m_textureDesc;

    public:
        TextureResource(GraphicsCore& aCore, eastl::string_view aName, cppfs::FilePath aFullPath,
                        nvrhi::TextureDesc aTextureDesc);

        [[nodiscard]]
        nvrhi::ITexture* GetResource() override;

        bool Reload() override;
        void Unload() override;
        void Destroy() override;
    };
}

#endif // SOGE_GRAPHICS_RESOURCES_TEXTURERESOURCE_HPP
