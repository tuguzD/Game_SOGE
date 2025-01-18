#ifndef SOGE_GRAPHICS_VERTEX_HPP
#define SOGE_GRAPHICS_VERTEX_HPP

#include <glm/glm.hpp>


namespace soge
{
    struct Vertex final
    {
        alignas(16) glm::vec3 m_position;
        glm::vec3 m_normal;
        glm::vec4 m_color;
    };
}

#endif // !SOGE_GRAPHICS_VERTEX_HPP
