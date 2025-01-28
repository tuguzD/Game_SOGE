#ifndef GAME_PIECE_HPP
#define GAME_PIECE_HPP

#include "SOGE/Utils/UUID.hpp"


namespace soge_game
{
    struct Piece final
    {
    public:
        UUIDv4::UUID uuid;
        bool darkTeam;
    };
}

#endif // GAME_PIECE_HPP
