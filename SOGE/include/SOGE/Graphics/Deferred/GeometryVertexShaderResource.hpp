#ifndef SOGE_GRAPHICS_DEFERRED_GEOMETRYVERTEXSHADERRESOURCE_HPP
#define SOGE_GRAPHICS_DEFERRED_GEOMETRYVERTEXSHADERRESOURCE_HPP

#include "SOGE/Graphics/Resources/ShaderResource.hpp"


namespace soge
{
    class GeometryVertexShaderResource : public ShaderResource
    {
    public:
        explicit GeometryVertexShaderResource(GraphicsCore& aCore);
    };
}

SOGE_DI_REGISTER_NS(soge, GeometryVertexShaderResource, df::Single<GeometryVertexShaderResource, GraphicsCore>)

#endif // SOGE_GRAPHICS_DEFERRED_GEOMETRYVERTEXSHADERRESOURCE_HPP
