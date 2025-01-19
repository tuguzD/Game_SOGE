#ifndef SOGE_GRAPHICS_PRIMITIVES_BOXPRIMITIVE_HPP
#define SOGE_GRAPHICS_PRIMITIVES_BOXPRIMITIVE_HPP

#include "SOGE/Graphics/Primitives/PrimitiveBase.hpp"
#include <glm/vec3.hpp>


namespace soge
{
    class BoxPrimitive final : public PrimitiveBase
    {
    public:
        BoxPrimitive(const glm::vec3& aSize = glm::vec3{1.0f}, const glm::vec4& aColor = glm::vec4{1.0f});
        ~BoxPrimitive();

    };
}

#endif // !SOGE_GRAPHICS_PRIMITIVES_BOXPRIMITIVE_HPP
