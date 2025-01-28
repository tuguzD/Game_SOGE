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

        static constexpr std::float_t height = 0.26715f * 2;
    };
}

#endif // GAME_PIECE_HPP
