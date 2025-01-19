#include "sogepch.hpp"
#include "SOGE/Graphics/Primitives/BoxPrimitive.hpp"
#include "SOGE/Graphics/TriangleEntity.hpp"
#include "SOGE/Graphics/Mesh.hpp"


namespace soge
{
    BoxPrimitive::BoxPrimitive(const glm::vec3& aSize, const glm::vec4& aColor)
    {
        constexpr glm::vec4 red{0.91f, 0.36f, 0.36f, 1.0f};
        constexpr glm::vec4 green{0.37f, 0.82f, 0.47f, 1.0f};
        constexpr glm::vec4 blue{0.42f, 0.49f, 0.95f, 1.0f};
        constexpr glm::vec4 gray{0.28f, 0.28f, 0.28f, 1.0f};

        const float right       = aSize.x / 2;
        const float left        = -aSize.x / 2;
        const float top         = aSize.y / 2;
        const float bottom      = -aSize.y / 2;
        const float forward     = aSize.z / 2;
        const float backward    = -aSize.z / 2;

        m_primitiveMesh->CreateVertexBuffer({
            // Front face

            Vertex{glm::vec3{left, bottom, forward}, aColor * blue /*, glm::vec2{1.0f, 1.0f}*/},
            Vertex{glm::vec3{right, bottom, forward}, aColor * green /*, glm::vec2{0.0f, 1.0f}*/},
            Vertex{glm::vec3{right, top, forward}, aColor * red /*, glm::vec2{0.0f, 0.0f}*/},
            Vertex{glm::vec3{left, top, forward}, aColor * gray /*, glm::vec2{1.0f, 0.0f}*/},

            // Back face

            Vertex{glm::vec3{left, bottom, backward}, aColor * red /*, glm::vec2{0.0f, 1.0f}*/},
            Vertex{glm::vec3{left, top, backward}, aColor * green /*, glm::vec2{0.0f, 0.0f}*/},
            Vertex{glm::vec3{right, top, backward}, aColor * blue /*, glm::vec2{1.0f, 0.0f}*/},
            Vertex{glm::vec3{right, bottom, backward}, aColor * gray /*, glm::vec2{1.0f, 1.0f}*/},

            // Top face

            Vertex{glm::vec3{left, top, backward}, aColor * green /*, glm::vec2{0.0f, 1.0f}*/},
            Vertex{glm::vec3{left, top, forward}, aColor * gray /*, glm::vec2{0.0f, 0.0f}*/},
            Vertex{glm::vec3{right, top, forward}, aColor * red /*, glm::vec2{1.0f, 0.0f}*/},
            Vertex{glm::vec3{right, top, backward}, aColor * blue /*, glm::vec2{1.0f, 1.0f}*/},

            // Bottom face

            Vertex{glm::vec3{left, bottom, backward}, aColor * red /*, glm::vec2{1.0f, 1.0f}*/},
            Vertex{glm::vec3{right, bottom, backward}, aColor * gray /*, glm::vec2{0.0f, 1.0f}*/},
            Vertex{glm::vec3{right, bottom, forward}, aColor * green /*, glm::vec2{0.0f, 0.0f}*/},
            Vertex{glm::vec3{left, bottom, forward}, aColor * blue /*, glm::vec2{1.0f, 0.0f}*/},

            // Left face

            Vertex{glm::vec3{left, bottom, forward}, aColor * blue /*, glm::vec2{0.0f, 1.0f}*/},
            Vertex{glm::vec3{left, top, forward}, aColor * gray /*, glm::vec2{0.0f, 0.0f}*/},
            Vertex{glm::vec3{left, top, backward}, aColor * green /*, glm::vec2{1.0f, 0.0f}*/},
            Vertex{glm::vec3{left, bottom, backward}, aColor * red /*, glm::vec2{1.0f, 1.0f}*/},

            // Right face

            Vertex{glm::vec3{right, bottom, backward}, aColor * gray /*, glm::vec2{0.0f, 1.0f}*/},
            Vertex{glm::vec3{right, top, backward}, aColor * blue /*, glm::vec2{0.0f, 0.0f}*/},
            Vertex{glm::vec3{right, top, forward}, aColor * red /*, glm::vec2{1.0f, 0.0f}*/},
            Vertex{glm::vec3{right, bottom, forward}, aColor * green /*, glm::vec2{1.0f, 1.0f}*/}
        });

        m_primitiveMesh->CreateIndexBuffer({

            // Front face

            0u, 1u, 2u,
            0u, 2u, 3u,

            // Back face

            4u, 5u, 6u,
            4u, 6u, 7u,

            // Top Face

            8u, 9u, 10u,
            8u, 10u, 11u,

            // Bottom Face

            12u, 13u, 14u,
            12u, 14u, 15u,

            // Left Face

            16u, 17u, 18u,
            16u, 18u, 19u,

            // Right Face

            20u, 21u, 22u,
            20u, 22u, 23u,
        });
    }

    BoxPrimitive::~BoxPrimitive()
    {
    }
}
