#ifndef SOGE_GRAPHICS_GENERIC_VERTEX_HPP
#define SOGE_GRAPHICS_GENERIC_VERTEX_HPP

#include <glm/vec4.hpp>


namespace soge
{
    struct Vertex
    {
        glm::vec4 m_position;
        glm::vec4 m_color;
    };
}

#endif // SOGE_GRAPHICS_GENERIC_VERTEX_HPP
