#ifndef GAME_CURSOR_HPP
#define GAME_CURSOR_HPP

#include "Board.hpp"
#include "SOGE/Graphics/GraphicsModule.hpp"
#include "SOGE/Graphics/Primitives/Box.hpp"

#include <SOGE/Utils/UUID.hpp>


namespace soge_game
{
    class Cursor final
    {
    public:
        UUIDv4::UUID uuid{};
        bool darkTeam;

        static constexpr std::float_t height = Piece::height / 3;

        void init(soge::GraphicsEntityManager& entities, soge::di::Container& container)
        {
            const auto [cursorEntity, cursorUuid] = entities.CreateEntity<soge::BoxPrimitive>(
                container.Provide<soge::BoxPrimitive>());
            SOGE_APP_INFO_LOG(R"(Created box with UUID {})", cursorUuid.str());
            cursorEntity.GetTransform() = soge::Transform{
                .m_position = glm::vec3{
                    Board::get_coords(darkTeam, 0),
                    height, Board::get_coords(darkTeam, 0),
                },
                .m_scale = glm::vec3{0.725f, 0.1f, 0.725f},
            };
            uuid = cursorUuid;
        }

        void move(const soge::GraphicsEntityManager &entities, int vert = 0, int horz = 0, const bool log = false)
        {
            const auto entity = dynamic_cast<soge::BoxPrimitive*>(entities.GetEntity(uuid));
            if (entity == nullptr) return;

            auto z = Board::get_cell(darkTeam, entity->GetTransform().m_position.z);
            auto x = Board::get_cell(darkTeam, entity->GetTransform().m_position.x);

            z = Board::clamp_cell(z + vert);
            entity->GetTransform().m_position.z = Board::get_coords(darkTeam, z);

            x = Board::clamp_cell(x + horz);
            entity->GetTransform().m_position.x = Board::get_coords(darkTeam, x);

            if (log)
            {
                auto name = darkTeam ? "dark" : "light";
                SOGE_APP_INFO_LOG(R"(Current "{}" cursor location: ({}, {}))", name, x, z);
            }
        }

        void color(const soge::GraphicsEntityManager &entities, const Board& board)
        {
            const auto cursor = dynamic_cast<soge::BoxPrimitive*>(entities.GetEntity(uuid));
            if (cursor == nullptr) return;

            auto cell_x = Board::get_cell(false, cursor->GetTransform().m_position.x);
            auto cell_z = Board::get_cell(false, cursor->GetTransform().m_position.z);

            if (entities.GetEntity(board.matrix[cell_x][cell_z].uuid))
            {
                if (board.matrix[cell_x][cell_z].darkTeam == darkTeam)
                    // color "green"
                    cursor->GetMaterial().m_diffuse = glm::vec3{0.0f, 0.75f, 0.0f};
                else // color "red"
                    cursor->GetMaterial().m_diffuse = glm::vec3{0.75f, 0.0f, 0.0f};
            } // color "white"
            else cursor->GetMaterial().m_diffuse = glm::vec3{0.75f};
        }
    };
}

#endif // GAME_CURSOR_HPP
