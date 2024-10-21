#include "sogepch.hpp"

#include "SOGE/Core/Engine.hpp"
#include "SOGE/Core/Timestep.hpp"


namespace soge
{
    Engine* Engine::s_instance = nullptr;
    Engine* Engine::GetInstance()
    {
        if (s_instance == nullptr)
        {
            s_instance = new Engine();
        }

        return s_instance;
    }

    Engine::Engine()
    {
        SOGE_INFO_LOG("Initialize engine...");

        m_isRunning = false;
    }

    void Engine::Run()
    {
        m_isRunning = true;
        while (m_isRunning)
        {
            Timestep::StartFrame();
            Timestep::CalculateDelta();
            SOGE_INFO_LOG("DeltaTime: {0}", Timestep::RealDeltaTime());
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
        m_isRunning = false;
    }

    void Engine::Shutdown()
    {
    }
}
