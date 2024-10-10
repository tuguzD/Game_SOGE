#ifndef SOGE_ENGINE_HPP
#define SOGE_ENGINE_HPP


namespace soge
{
    class Engine
    {
    private:
        bool mIsRunning;

    protected:
        Engine();
        static Engine* sInstance;

        void Shutdown();

    public:
        Engine(Engine&)         = delete;
        void operator=(Engine&) = delete;

        void Run();
        void Update();
        void FixedUpdate();
        void RequestShutdown();

    public:
        static Engine* GetInstance();

    };

    Engine* CreateApplication();
}

#endif // !SOGE_ENGINE_HPP
