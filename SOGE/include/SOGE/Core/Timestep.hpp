#ifndef SOGE_CORE_TIMESTEP_HPP
#define SOGE_CORE_TIMESTEP_HPP


namespace soge
{
    namespace chrono = eastl::chrono;

    class Timestep final
    {
        friend class Engine;
        using ClockEngine = chrono::high_resolution_clock;

    private:
        static std::uint64_t s_frameTime;
        static std::uint64_t s_frameTimeForRealDelta;
        static float s_deltaTime;
        static float s_realDeltaTime;
        static float s_globalTime;

    private:
        static void StartFrame();
        static void CalculateDelta();

    public:
        static void SetDelta(float aDelta);

        static float DeltaTime();
        static float RealDeltaTime();
        static float FrameTime();

        static std::uint64_t GetMilliseconds();
        static std::uint64_t GetMicroseconds();
        static std::uint64_t GetNanoseconds();
        static std::uint64_t GetSeconds();
    };
}

#endif // SOGE_CORE_TIMESTEP_HPP
