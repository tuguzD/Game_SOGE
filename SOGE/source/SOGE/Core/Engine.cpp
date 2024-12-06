#include "sogepch.hpp"

#include "SOGE/Core/Engine.hpp"
#include "SOGE/Core/Timestep.hpp"
#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Input/InputModule.hpp"


namespace soge
{
    UniquePtr<Engine> Engine::s_instance(nullptr);
    std::mutex Engine::s_mutex;

    Engine* Engine::GetInstance()
    {
        // Fast path: return the instance if it is already initialized
        if (s_instance != nullptr)
        {
            return s_instance.get();
        }

        // Safe but slow path: initialize with default class if empty
        std::lock_guard lock(s_mutex);
        // Additional check to ensure we are creating new instance exactly once
        if (s_instance == nullptr)
        {
            // Replicating `make_unique` here because the constructor is protected
            s_instance = UniquePtr<Engine>(new Engine());
        }
        return s_instance.get();
    }

    Engine::Engine() : m_isRunning(false), m_shutdownRequested(false)
    {
        SOGE_INFO_LOG("Initialize engine...");

        CreateModule<EventModule>();
        CreateModule<InputModule>();

        m_systemWindow = UniquePtr<Window>(Window::Create());
    }

    Engine::~Engine()
    {
        SOGE_INFO_LOG("Destroy engine...");
    }

    void Engine::Run()
    {
        if (m_isRunning)
        {
            return;
        }

        // Prevent users from resetting engine while it is running
        std::lock_guard lock(s_mutex);

        m_isRunning = true;
        for (Module& module : m_moduleManager)
        {
            module.Load(m_container, m_moduleManager);
        }

        m_shutdownRequested = false;
        while (!m_shutdownRequested)
        {
            Timestep::StartFrame();
            Timestep::CalculateDelta();

            const auto eventModule = GetModule<EventModule>();
            eventModule->Dispatch<UpdateEvent>(Timestep::DeltaTime());
        }

        for (Module& module : m_moduleManager)
        {
            module.Unload(m_container, m_moduleManager);
        }
        m_isRunning = false;
        m_removedModules.clear();
    }

    bool Engine::IsRunning() const
    {
        return m_isRunning;
    }

    void Engine::RequestShutdown()
    {
        m_shutdownRequested = true;
    }

    di::Container& Engine::GetDependencyContainer()
    {
        return m_container;
    }

    di::Provider& Engine::GetDependencyProvider()
    {
        return m_container;
    }
}
