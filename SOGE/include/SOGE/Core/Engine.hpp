#ifndef SOGE_CORE_ENGINE_HPP
#define SOGE_CORE_ENGINE_HPP

#include "SOGE/System/Memory.hpp"


namespace soge
{
    class Engine
    {
    public:
        Engine(Engine&) = delete;
        auto operator=(Engine&) = delete;

        virtual ~Engine();

        static Engine* GetInstance();

        template <typename T = Engine, typename... Args>
        static T* Reset(Args&&... args);

        void Run();
        void Update();
        void FixedUpdate();
        void RequestShutdown();

    protected:
        explicit Engine();

        void Shutdown();

    private:
        static UniquePtr<Engine> s_instance;
        static std::mutex s_init_mutex;

        bool m_isRunning;
    };

    template <typename T, typename... Args>
    T* Engine::Reset(Args&&... args)
    {
        // Replicating `make_unique` here because the constructor is protected
        UniquePtr<T> newInstance(new T(std::forward<Args>(args)...));
        // Save pointer to `T` to avoid dynamic cast later
        T* pNewInstance = newInstance.get();

        // Acquire lock just before moving the new instance
        // (this allows to allocate new instance in parallel compared to acquiring at the beginning of the function)
        std::lock_guard lock(s_init_mutex);
        // Move new instance to the static instance
        s_instance = std::move(newInstance);
        // Return previously saved pointer
        return pNewInstance;
    }

    Engine* CreateApplication();
}

#endif // SOGE_CORE_ENGINE_HPP
