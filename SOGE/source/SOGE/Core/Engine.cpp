#include "sogepch.hpp"
#include "SOGE/Core/Engine.hpp"


namespace soge
{
    Engine* Engine::sInstance = nullptr;
    Engine* Engine::GetInstance() {
        if (sInstance == nullptr) {
            sInstance = new Engine();
        }

        return sInstance;
    }

    Engine::Engine()
    {
        SOGE_INFO_LOG("Initialize engine...");

        mIsRunning = false;
    }

    void Engine::Run()
    {
        mIsRunning = true;
        while (mIsRunning) {

        }

        this->Shutdown();
    }

    void Engine::RequestShutdown()
    {
        mIsRunning = false;
    }

    void Engine::Shutdown()
    {
    }
}