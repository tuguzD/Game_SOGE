#include "sogepch.hpp"
#include "SOGE/Core/Engine.hpp"
#include <EASTL/vector.h>

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
        eastl::vector<int> vec;
        vec.push_back(1);
        SOGE_INFO_LOG("{0}", vec[0]);
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