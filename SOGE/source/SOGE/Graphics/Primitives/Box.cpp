#include "sogepch.hpp"

#include "SOGE/Graphics/Primitives/Box.hpp"


namespace soge
{
    GeometryEntity CreateBox(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline, const CreateBoxParams& aParams)
    {
        const auto vertices = CreateBoxVertices(aParams.m_dimensions, aParams.m_color);
        eastl::vector<GeometryEntity::Vertex> verticesVector;
        verticesVector.assign(vertices.begin(), vertices.end());

        constexpr auto indices = CreateBoxIndices();
        eastl::vector<GeometryEntity::Index> indicesVector;
        indicesVector.assign(indices.begin(), indices.end());

        return GeometryEntity{
            aCore, aPipeline, aParams.m_transform, std::move(verticesVector), std::move(indicesVector),
        };
    }
}
