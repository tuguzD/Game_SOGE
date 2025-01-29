#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include "Piece.hpp"

#include <SOGE/Utils/UUID.hpp>

namespace soge_game
{
    class Board
    {
    private:
        static constexpr float_t offset = 2.38f;

    public:
        static constexpr int8_t order = 8;
        Piece matrix[order][order]{};

        UUIDv4::UUID uuid{};

        void init(soge::GraphicsEntityManager& entities, soge::di::Container& container)
        {
            const auto [entity, id] = entities.CreateEntity<soge::StaticMeshEntity>(
                container.Provide<soge::StaticMeshEntity>());
            SOGE_APP_INFO_LOG(R"(Created board with UUID {})", id.str());
            entity.GetFilePath() = "./resources/meshes/board/board.fbx";

            entity.GetTransform() = soge::Transform{
                .m_rotation = glm::vec3{0.0f, glm::radians(90.0f), 0.0f},
                .m_scale = glm::vec3{0.1f},
            };
            entity.Load();
            this->uuid = id;

            for (std::size_t i = 0; i < 2; ++i)
            {
                const auto name = !i ? "light_piece" : "dark_piece";
                for (std::size_t j = 0; j < 3; ++j)
                    for (std::size_t k = 0; k < 4; ++k)
                    {
                        auto z = static_cast<int>(j);
                        auto x = z % 2 + static_cast<int>(k) * 2;

                        const auto coords_x = get_coords(i, x);
                        const auto coords_z = get_coords(i, z);
                        x = get_cell(false, coords_x);
                        z = get_cell(false, coords_z);

                        matrix[x][z] = Piece{.darkTeam = static_cast<bool>(i)};
                        matrix[x][z].init(name, {coords_x, coords_z}, entities, container);
                    }
            }
        }

        static float get_coords(const bool darkTeam, int cell)
        {
            const auto mult_offset = offset * static_cast<float_t>(!darkTeam ? -1 : 1);
            const auto temp = static_cast<float_t>(clamp_cell(cell));

            const auto result = mult_offset * (1 - temp * 2 / (order - 1));
            return result;
        }

        static int get_cell(const bool darkTeam, float_t coords)
        {
            const auto mult_offset = offset * static_cast<float_t>(!darkTeam ? -1 : 1);

            const auto result = (1 - coords / mult_offset) * (order - 1) / 2;
            return static_cast<int>(round(result));
        }

        static int clamp_cell(int cell)
        {
            return glm::clamp(cell, 0, order - 1);
        }
    };
}

#endif // GAME_BOARD_HPP
