#ifndef SOGE_CORE_TIMESTEP_HPP
#define SOGE_CORE_TIMESTEP_HPP


namespace soge
{
    namespace _ChronoNSpace = eastl::chrono;

    class Timestep final
    {
        friend class Engine;
        using _ClockEngine = _ChronoNSpace::high_resolution_clock;

    private:
        static std::uint64_t mFrameTime;
        static std::uint64_t mFrameTimeForRealDelta;
        static float mDeltaTime;
        static float mRealDeltaTime;
        static float mGlobalTime;

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
