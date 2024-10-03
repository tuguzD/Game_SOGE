#include "sogepch.hpp"
#include "SOGE/Core/Engine.hpp"
#include "SOGE/Core/Timestep.hpp"


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
            Timestep::StartFrame();
            Timestep::CalculateDelta();
        }

        this->Shutdown();
    }

    void Engine::Update()
    {
        FixedUpdate();
    }

    void Engine::FixedUpdate()
    {

    }

    void Engine::RequestShutdown()
    {
        mIsRunning = false;
    }

    void Engine::Shutdown()
    {
    }
}