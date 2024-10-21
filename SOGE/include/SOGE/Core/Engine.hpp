#ifndef SOGE_CORE_ENGINE_HPP
#define SOGE_CORE_ENGINE_HPP


namespace soge
{
    class Engine
    {
    private:
        bool m_isRunning;

    protected:
        Engine();
        static Engine* s_instance;

        void Shutdown();

    public:
        Engine(Engine&) = delete;
        auto operator=(Engine&) = delete;

        void Run();
        void Update();
        void FixedUpdate();
        void RequestShutdown();

    public:
        static Engine* GetInstance();
    };

    Engine* CreateApplication();
}

#endif // SOGE_CORE_ENGINE_HPP
