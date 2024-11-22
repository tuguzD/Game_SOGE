#include "sogepch.hpp"

#include "SOGE/Core/Engine.hpp"
#include "SOGE/Core/Timestep.hpp"
#include "SOGE/Input/InputTypes.hpp"

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

        Keys::Initialize();
        m_eventManager = CreateUnique<EventManager>();
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

        //SOGE_INFO_LOG("Caps Lock says: {0}", Keys::CapsLock.IsMouseButton());
        SOGE_INFO_LOG("RMB is mouse button? {0}", Keys::RightMouseButton.GetAlternateName().c_str());

        m_shutdownRequested = false;
        while (!m_shutdownRequested)
        {
            Timestep::StartFrame();
            Timestep::CalculateDelta();
        }

        m_isRunning = false;
    }

    bool Engine::IsRunning() const
    {
        return m_isRunning;
    }

    void Engine::RequestShutdown()
    {
        m_shutdownRequested = true;
    }
}
