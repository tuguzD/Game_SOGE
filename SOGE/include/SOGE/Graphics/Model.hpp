#ifndef SOGE_GRAPHICS_MODEL_HPP
#define SOGE_GRAPHICS_MODEL_HPP

#include "SOGE/Graphics/Mesh.hpp"
#include "SOGE/Graphics/Resources/ModelResource.hpp"


namespace soge
{
    class Model final
    {
    private:
        eastl::vector<Mesh> m_meshes;
        ModelResource* m_modelResource;

    public:
        Model(ModelResource* aModelResource);
        ~Model();

    };
}

#endif // !SOGE_GRAPHICS_MODEL_HPP
