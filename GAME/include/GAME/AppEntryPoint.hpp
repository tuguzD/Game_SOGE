#ifndef GAME_GAME_ENTRY_POINT_HPP
#define GAME_GAME_ENTRY_POINT_HPP

#include <SOGE/SOGE.hpp>
#include <SOGE/Core/EntryPoint.hpp>


namespace game
{
    class Game final : public soge::Engine
    {
    public:
        Game();
        ~Game();

    };
}

#endif // !GAME_GAME_ENTRY_POINT_HPP
