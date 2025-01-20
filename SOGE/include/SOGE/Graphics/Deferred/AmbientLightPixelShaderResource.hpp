#ifndef SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTPIXELSHADERRESOURCE_HPP
#define SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTPIXELSHADERRESOURCE_HPP

#include "SOGE/Graphics/Resources/ShaderResource.hpp"


namespace soge
{
    class AmbientLightPixelShaderResource : public ShaderResource
    {
    public:
        explicit AmbientLightPixelShaderResource(GraphicsCore& aCore);
    };
}

SOGE_DI_REGISTER_NS(soge, AmbientLightPixelShaderResource, df::Single<AmbientLightPixelShaderResource, GraphicsCore>)

#endif // SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTPIXELSHADERRESOURCE_HPP
