#ifndef SOGE_CORE_ENGINE_HPP
#define SOGE_CORE_ENGINE_HPP

#include "SOGE/Core/DI/Container.hpp"
#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/System/Memory.hpp"


namespace soge
{
    class Engine;

    template <typename T>
    concept DerivedFromEngine = std::derived_from<T, Engine>;

    class Engine
    {
    private:
        static UniquePtr<Engine> s_instance;
        static std::mutex s_mutex;

        bool m_isRunning;
        bool m_shutdownRequested;

        di::Container m_container;
        ModuleManager m_moduleManager;

    protected:
        explicit Engine();

        di::Container& GetDependencyContainer();

    public:
        Engine(const Engine&) = delete;
        auto operator=(const Engine&) = delete;

        Engine(Engine&&) = delete;
        auto operator=(Engine&&) = delete;

        virtual ~Engine();

        static Engine* GetInstance();

        template <DerivedFromEngine T = Engine, typename... Args>
        static T* Reset(Args&&... args);

        [[nodiscard]]
        bool IsRunning() const;

        void Run();
        void RequestShutdown();

        di::Provider& GetDependencyProvider();

        template <DerivedFromModule T, typename... Args>
        T& CreateModule(Args&&... args);

        template <DerivedFromModule T>
        [[nodiscard]]
        T* GetModule() const;
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

    template <DerivedFromModule T, typename... Args>
    T& Engine::CreateModule(Args&&... args)
    {
        auto [module, created] = m_moduleManager.CreateModule<T>(std::forward<Args>(args)...);
        if constexpr (di::ModuleDependency<T>)
        {
            m_container.Create<T>(module);
        }
        if (created && m_isRunning)
        {
            module.Load(m_container);
        }

        return module;
    }

    template <DerivedFromModule T>
    T* Engine::GetModule() const
    {
        return m_moduleManager.GetModule<T>();
    }

    Engine* CreateApplication();
}

#endif // SOGE_CORE_ENGINE_HPP
