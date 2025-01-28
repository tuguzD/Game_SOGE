#ifndef GAME_PIECE_HPP
#define GAME_PIECE_HPP

#include <SOGE/Utils/UUID.hpp>


namespace soge_game
{
    class Piece final
    {
    public:
        UUIDv4::UUID uuid;
        bool darkTeam;

        static constexpr std::int8_t matrix_order = 8;
        static constexpr std::float_t height = 0.26715f * 2;

        static float get_coords(const bool darkTeam, int cell)
        {
            const auto offset = 2.38f * static_cast<float_t>(!darkTeam ? -1 : 1);

            const auto temp = static_cast<float_t>(clamp_cell(cell));
            const auto result = offset * (1 - temp*2 / (matrix_order - 1));
            return result;
        }

        static int get_cell(const bool darkTeam, float_t coords)
        {
            const auto offset = 2.38f * static_cast<float_t>(!darkTeam ? -1 : 1);

            const auto result = (1 - coords / offset) * (matrix_order - 1) / 2;
            return static_cast<int>(round(result));
        }

        static int clamp_cell(int cell)
        {
            return std::max(0, std::min(cell, matrix_order - 1));
        }
    };
}

#endif // GAME_PIECE_HPP
