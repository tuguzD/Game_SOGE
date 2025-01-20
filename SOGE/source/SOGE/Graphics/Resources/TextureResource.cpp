#include "sogepch.hpp"

#include "SOGE/Graphics/Resources/TextureResource.hpp"


namespace soge
{
    TextureResource::TextureResource(GraphicsCore& aCore, const eastl::string_view aName, cppfs::FilePath aFullPath,
                                     nvrhi::TextureDesc aTextureDesc)
        : GraphicsResource{aName, std::move(aFullPath)}, m_core{aCore}
    {
        // TODO
    }

    nvrhi::ITexture* TextureResource::GetResource()
    {
        return m_textureHandle;
    }

    bool TextureResource::Reload()
    {
        // TODO
        return false;
    }

    void TextureResource::Unload()
    {
        // TODO
    }

    void TextureResource::Destroy()
    {
        // TODO
    }
}
