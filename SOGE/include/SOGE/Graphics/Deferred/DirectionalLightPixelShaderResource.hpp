#ifndef SOGE_GRAPHICS_DEFERRED_DIRECTIONALLIGHTPIXELSHADERRESOURCE_HPP
#define SOGE_GRAPHICS_DEFERRED_DIRECTIONALLIGHTPIXELSHADERRESOURCE_HPP

#include "SOGE/Graphics/Resources/ShaderResource.hpp"


namespace soge
{
    class DirectionalLightPixelShaderResource : public ShaderResource
    {
    public:
        explicit DirectionalLightPixelShaderResource(GraphicsCore& aCore);
    };
}

SOGE_DI_REGISTER_NS(soge, DirectionalLightPixelShaderResource,
                    df::Single<DirectionalLightPixelShaderResource, GraphicsCore>)

#endif // SOGE_GRAPHICS_DEFERRED_DIRECTIONALLIGHTPIXELSHADERRESOURCE_HPP
