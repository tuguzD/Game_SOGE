#ifndef SOGE_STOPWATCH_HPP
#define SOGE_STOPWATCH_HPP


namespace soge
{
    namespace _ChronoNSpace = eastl::chrono;

    class TimeUnit final
    {
        using _ReturnType = unsigned long long;

    private:
        std::uint64_t mNanoCount;

    public:
        TimeUnit();
        TimeUnit(_ChronoNSpace::nanoseconds aDuration);
        ~TimeUnit();

        const _ReturnType Milliseconds() const;
        const _ReturnType Nanoseconds() const;
        const _ReturnType Microseconds() const;
        const _ReturnType Seconds() const;

        TimeUnit& operator= (const TimeUnit& aOther);
        constexpr bool operator== (const TimeUnit& aOther);
        TimeUnit& operator= (const _ChronoNSpace::nanoseconds& aNano);
        constexpr bool operator== (const std::uint64_t& aValue);

    };

    class Stopwatch
    {
        using _ClockEngine = _ChronoNSpace::high_resolution_clock;
        using _TimePoint = _ChronoNSpace::time_point<_ClockEngine>;

    private:
        _TimePoint mStartTimePoint;
        _TimePoint mEndTimePoint;
        TimeUnit mUnits;
        bool mIsStopped;

    private:
        void CalculateDuration();

    public:
        Stopwatch();
        ~Stopwatch();

        void Start() noexcept;
        void Stop(bool aCalculateAtStop=true) noexcept;
        void Restart() noexcept;

        const TimeUnit Elapsed();

    };
}

#endif // !SOGE_STOPWATCH_HPP
