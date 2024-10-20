#include "GAME/AppEntryPoint.hpp"


namespace game
{
    Game::Game()
    {
        SOGE_APP_INFO_LOG("Initialize game...");
    }

    Game::~Game()
    {
    }
}

soge::Engine* soge::CreateApplication()
{
    return new game::Game();
}
