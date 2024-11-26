#ifndef SOGE_CORE_ENGINE_HPP
#define SOGE_CORE_ENGINE_HPP

#include "SOGE/System/Memory.hpp"
#include "SOGE/Core/EventManager.hpp"
#include "SOGE/Input/InputManager.hpp"


namespace soge
{
    class Engine;

    template <typename T>
    concept DerivedFromEngine = std::is_base_of_v<Engine, T>;

    class Engine
    {
    private:
        static UniquePtr<Engine> s_instance;
        static std::mutex s_mutex;

        eastl::unique_ptr<EventManager> m_eventManager;
        SharedPtr<InputManager> m_inputManager;

        bool m_isRunning;
        bool m_shutdownRequested;

    protected:
        explicit Engine();

    public:
        Engine(const Engine&) = delete;
        auto operator=(const Engine&) = delete;

        Engine(Engine&&) = delete;
        auto operator=(Engine&&) = delete;

        virtual ~Engine();

        void Run();
        void RequestShutdown();

        [[nodiscard]]
        bool IsRunning() const;

        EventManager* GetEventManager() const;

    public:
        static Engine* GetInstance();
        template <DerivedFromEngine T = Engine, typename... Args>
        static T* Reset(Args&&... args);

    };

    template <DerivedFromEngine T, typename... Args>
    T* Engine::Reset(Args&&... args)
    {
        // Replicating `make_unique` here because the constructor is protected
        UniquePtr<T> newInstance(new T(std::forward<Args>(args)...));
        // Save pointer to `T` to avoid dynamic cast later
        T* pNewInstance = newInstance.get();

        // Acquire lock just before moving the new instance
        // (this allows to allocate new instance in parallel compared to acquiring at the beginning of the function)
        std::lock_guard lock(s_mutex);
        // Move new instance to the static instance
        s_instance = std::move(newInstance);
        // Return previously saved pointer
        return pNewInstance;
    }

    Engine* CreateApplication();
}

#endif // SOGE_CORE_ENGINE_HPP
