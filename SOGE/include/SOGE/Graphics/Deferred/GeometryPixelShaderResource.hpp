#ifndef SOGE_GRAPHICS_DEFERRED_GEOMETRYPIXELSHADERRESOURCE_HPP
#define SOGE_GRAPHICS_DEFERRED_GEOMETRYPIXELSHADERRESOURCE_HPP

#include "SOGE/Graphics/Resources/ShaderResource.hpp"


namespace soge
{
    class GeometryPixelShaderResource : public ShaderResource
    {
    public:
        explicit GeometryPixelShaderResource(GraphicsCore& aCore);
    };
}

SOGE_DI_REGISTER_NS(soge, GeometryPixelShaderResource, df::Single<GeometryPixelShaderResource, GraphicsCore>)

#endif // SOGE_GRAPHICS_DEFERRED_GEOMETRYPIXELSHADERRESOURCE_HPP
