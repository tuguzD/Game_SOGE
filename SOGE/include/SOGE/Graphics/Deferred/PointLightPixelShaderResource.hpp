#ifndef SOGE_GRAPHICS_DEFERRED_POINTLIGHTPIXELSHADERRESOURCE_HPP
#define SOGE_GRAPHICS_DEFERRED_POINTLIGHTPIXELSHADERRESOURCE_HPP

#include "SOGE/Graphics/Resources/ShaderResource.hpp"


namespace soge
{
    class PointLightPixelShaderResource : public ShaderResource
    {
    public:
        explicit PointLightPixelShaderResource(GraphicsCore& aCore);
    };
}

SOGE_DI_REGISTER_NS(soge, PointLightPixelShaderResource, df::Single<PointLightPixelShaderResource, GraphicsCore>)

#endif // SOGE_GRAPHICS_DEFERRED_POINTLIGHTPIXELSHADERRESOURCE_HPP
