#ifndef GAME_PIECE_HPP
#define GAME_PIECE_HPP

#include "SOGE/Graphics/GraphicsEntityManager.hpp"
#include "SOGE/Graphics/Entities/StaticMeshEntity.hpp"

#include <SOGE/Utils/UUID.hpp>


namespace soge_game
{
    class Piece final
    {
    public:
        bool darkTeam;
        UUIDv4::UUID uuid{};

        static constexpr std::float_t height = 0.26715f * 2;

        void init(glm::vec2 coords, soge::GraphicsEntityManager& entities, soge::di::Container& container)
        {
            const auto [entity, id] = entities.CreateEntity<soge::StaticMeshEntity>(
                container.Provide<soge::StaticMeshEntity>());

            std::string name = !darkTeam ? "light" : "dark";
            entity.GetFilePath() = std::format(
                "./resources/meshes/{}/{}.fbx", name + "_piece", name + "_piece");

            entity.GetTransform() = soge::Transform{
                .m_position = glm::vec3{coords.x, height, coords.y},
                .m_scale = glm::vec3{0.1f},
            };
            entity.Load();
            SOGE_APP_INFO_LOG(R"(Created "{}" piece with UUID {})", name, id.str());
            this->uuid = id;
        }
    };
}

#endif // GAME_PIECE_HPP
