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
        UUIDv4::UUID uuid;
        bool darkTeam;

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
