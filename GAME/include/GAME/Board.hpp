#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include "Piece.hpp"

#include <SOGE/Utils/UUID.hpp>

namespace soge_game
{
    class Board
    {
    public:
        static constexpr std::int8_t order = 8;
        Piece matrix[order][order]{};

        UUIDv4::UUID uuid{};

        void init(soge::GraphicsEntityManager& entities, soge::di::Container& container)
        {
            const auto [boardEntity, boardUuid] = entities.CreateEntity<soge::StaticMeshEntity>(
                container.Provide<soge::StaticMeshEntity>());
            SOGE_APP_INFO_LOG(R"(Created board with UUID {})", boardUuid.str());
            boardEntity.GetFilePath() = "./resources/meshes/board/board.fbx";

            boardEntity.GetTransform() = soge::Transform{
                .m_rotation = glm::vec3{0.0f, glm::radians(90.0f), 0.0f},
                .m_scale = glm::vec3{0.1f},
            };
            boardEntity.Load();
            uuid = boardUuid;
        }

        static float get_coords(const bool darkTeam, int cell)
        {
            const auto offset = 2.38f * static_cast<float_t>(!darkTeam ? -1 : 1);

            const auto temp = static_cast<float_t>(clamp_cell(cell));
            const auto result = offset * (1 - temp * 2 / (order - 1));
            return result;
        }

        static int get_cell(const bool darkTeam, float_t coords)
        {
            const auto offset = 2.38f * static_cast<float_t>(!darkTeam ? -1 : 1);

            const auto result = (1 - coords / offset) * (order - 1) / 2;
            return static_cast<int>(round(result));
        }

        static int clamp_cell(int cell)
        {
            return std::max(0, std::min(cell, order - 1));
        }
    };
}

#endif // GAME_BOARD_HPP
