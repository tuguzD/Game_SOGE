#ifndef SOGE_GRAPHICS_PRIMITIVES_PRIMITIVEBASE_HPP
#define SOGE_GRAPHICS_PRIMITIVES_PRIMITIVEBASE_HPP

#include "SOGE/Graphics/Mesh.hpp"
#include "SOGE/Utils/UUID.hpp"


namespace soge
{
    class PrimitiveBase
    {
    protected:
        UniquePtr<Mesh> m_primitiveMesh;
        UUIDv4::UUID m_uuid;

    public:
        virtual ~PrimitiveBase() = default;

    };
}

#endif // !SOGE_GRAPHICS_PRIMITIVES_PRIMITIVEBASE_HPP
