#include "GAME/AppEntryPoint.hpp"


namespace game
{
    Game::Game()
    {
        SOGE_APP_INFO_LOG("Initilize game...");
        SOGE_ERROR_LOG("Error");
    }

    Game::~Game()
    {
    }
}

soge::Engine* soge::CreateApplication() {
    return new game::Game();
}