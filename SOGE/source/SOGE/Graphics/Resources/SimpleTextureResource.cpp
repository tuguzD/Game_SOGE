#include "sogepch.hpp"

#include "SOGE/Graphics/Resources/SimpleTextureResource.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>


namespace soge
{
    SimpleTextureResource::SimpleTextureResource(GraphicsCore& aCore, const eastl::string_view aName,
                                                 cppfs::FilePath aFullPath)
        : TextureResource{aCore, aName, std::move(aFullPath), nvrhi::TextureDesc{}}, m_shouldWrite{true}
    {
        Initialize();
        SetLoaded(true);
    }

    eastl::span<const nvrhi::Color> SimpleTextureResource::GetPixels() const noexcept
    {
        return m_pixels;
    }

    void SimpleTextureResource::WriteResource(nvrhi::ICommandList& aCommandList)
    {
        if (!m_shouldWrite)
        {
            return;
        }
        m_shouldWrite = false;

        aCommandList.writeTexture(m_textureHandle, 0, 0, m_pixels.data(),
                                  static_cast<std::size_t>(m_textureDesc.width * 4));
    }

    void SimpleTextureResource::Initialize()
    {
        int width, height, channels;
        auto pixels = stbi_loadf(GetFullPath().data(), &width, &height, &channels, STBI_rgb_alpha);
        if (pixels == nullptr || channels != STBI_rgb_alpha)
        {
            const auto message = fmt::format(R"(Failed to load texture image by path "{}": {})", GetFullPath().data(),
                                             stbi_failure_reason());
            throw std::runtime_error{message};
        }

        m_pixels.assign(pixels, pixels + static_cast<std::ptrdiff_t>(width * height * channels));
        stbi_image_free(pixels);
        pixels = nullptr;

        m_textureDesc.width = width;
        m_textureDesc.height = height;
        m_textureDesc.format = nvrhi::Format::RGBA16_FLOAT;
        m_textureDesc.debugName =
            fmt::format(R"(SOGE 2D texture "{}" by path "{}")", GetName().data(), GetFullPath().data());

        m_textureHandle = m_core.get().GetRawDevice().createTexture(m_textureDesc);
        m_shouldWrite = true;
    }

    bool SimpleTextureResource::Reload()
    {
        if (m_textureHandle == nullptr)
        {
            Initialize();
            SetLoaded(true);
        }
        return true;
    }

    void SimpleTextureResource::Unload()
    {
        m_textureHandle = nullptr;
        m_pixels.clear();
        SetLoaded(false);
    }

    void SimpleTextureResource::Destroy()
    {
        m_textureHandle = nullptr;
        m_pixels.clear();
        SetLoaded(false);
    }
}
