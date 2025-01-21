#ifndef SOGE_GRAPHICS_RESOURCES_TEXTURERESOURCE_HPP
#define SOGE_GRAPHICS_RESOURCES_TEXTURERESOURCE_HPP

#include "SOGE/Content/ResourceBase.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class TextureResource : public GraphicsEntity, public ResourceBase
    {
    protected:
        eastl::reference_wrapper<GraphicsCore> m_core;

        nvrhi::TextureHandle m_textureHandle;
        nvrhi::TextureDesc m_textureDesc;

    public:
        TextureResource(GraphicsCore& aCore, eastl::string_view aName, cppfs::FilePath aFullPath,
                        nvrhi::TextureDesc aTextureDesc);

        [[nodiscard]]
        nvrhi::ITexture* GetTextureResource();
    };
}

#endif // SOGE_GRAPHICS_RESOURCES_TEXTURERESOURCE_HPP
