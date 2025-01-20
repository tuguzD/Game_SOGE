#ifndef SOGE_GRAPHICS_DEFERRED_DIRECTIONALLIGHTVERTEXSHADERRESOURCE_HPP
#define SOGE_GRAPHICS_DEFERRED_DIRECTIONALLIGHTVERTEXSHADERRESOURCE_HPP

#include "SOGE/Graphics/Resources/ShaderResource.hpp"


namespace soge
{
    class DirectionalLightVertexShaderResource : public ShaderResource
    {
    public:
        explicit DirectionalLightVertexShaderResource(GraphicsCore& aCore);
    };
}

SOGE_DI_REGISTER_NS(soge, DirectionalLightVertexShaderResource,
                    df::Single<DirectionalLightVertexShaderResource, GraphicsCore>)

#endif // SOGE_GRAPHICS_DEFERRED_DIRECTIONALLIGHTVERTEXSHADERRESOURCE_HPP
