#ifndef SOGE_GRAPHICS_DEFERRED_POINTLIGHTVERTEXSHADERRESOURCE_HPP
#define SOGE_GRAPHICS_DEFERRED_POINTLIGHTVERTEXSHADERRESOURCE_HPP

#include "SOGE/Graphics/Resources/ShaderResource.hpp"


namespace soge
{
    class PointLightVertexShaderResource : public ShaderResource
    {
    public:
        explicit PointLightVertexShaderResource(GraphicsCore& aCore);
    };
}

SOGE_DI_REGISTER_NS(soge, PointLightVertexShaderResource, df::Single<PointLightVertexShaderResource, GraphicsCore>)

#endif // SOGE_GRAPHICS_DEFERRED_POINTLIGHTVERTEXSHADERRESOURCE_HPP
