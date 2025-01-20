#ifndef SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTVERTEXSHADERRESOURCE_HPP
#define SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTVERTEXSHADERRESOURCE_HPP

#include "SOGE/Graphics/Resources/ShaderResource.hpp"


namespace soge
{
    class AmbientLightVertexShaderResource : public ShaderResource
    {
    public:
        explicit AmbientLightVertexShaderResource(GraphicsCore& aCore);
    };
}

SOGE_DI_REGISTER_NS(soge, AmbientLightVertexShaderResource, df::Single<AmbientLightVertexShaderResource, GraphicsCore>)

#endif // SOGE_GRAPHICS_DEFERRED_AMBIENTLIGHTVERTEXSHADERRESOURCE_HPP
