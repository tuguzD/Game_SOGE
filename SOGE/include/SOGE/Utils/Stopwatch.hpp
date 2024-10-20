#ifndef SOGE_UTILS_STOPWATCH_HPP
#define SOGE_UTILS_STOPWATCH_HPP


namespace soge
{
    // Alias to change chrono module, just in case...
    namespace chrono = eastl::chrono;

    /**
     * @class TimeUnit Stopwatch.hpp "SOGE/Utils/Stopwatch.hpp"
     * @brief Time unit translations class to quickly operate with time units.
     */
    class TimeUnit final
    {
        using ReturnType = unsigned long long;

    private:
        std::uint64_t mNanoCount;

    public:
        /**
         * @brief Constructs class with default value of 0 nanoseconds.
         */
        TimeUnit();

        /**
         * @brief Constructs class with given nanoseconds value.
         * @param[in] aDuration - Duration between start and end time points in nanoseconds.
         */
        TimeUnit(chrono::nanoseconds aDuration);
        ~TimeUnit();

        /**
         * @returns uint64_t value converted to milliseconds.
         */
        const ReturnType Milliseconds() const;

        /**
         * @returns Non converted class value.
         */
        const ReturnType Nanoseconds() const;

        /**
         * @returns uint64_t value converted to microseconds.
         */
        const ReturnType Microseconds() const;

        /**
         * @returns uint64_t value converted to seconds.
         */
        const ReturnType Seconds() const;

        TimeUnit& operator=(const TimeUnit& aOther);
        constexpr bool operator==(const TimeUnit& aOther);
        TimeUnit& operator=(const chrono::nanoseconds& aNano);
        constexpr bool operator==(const std::uint64_t& aValue);
    };

    /**
     * @class Stopwatch Stopwatch.hpp "SOGE/Utils/Stopwatch.hpp"
     * @brief Stopwatch class calculating duration between time points.
     * @todo Make it thread safe in the future...
     */
    class Stopwatch
    {
        using _ClockEngine = chrono::high_resolution_clock;
        using _TimePoint = chrono::time_point<_ClockEngine>;

    private:
        _TimePoint mStartTimePoint;
        _TimePoint mEndTimePoint;
        TimeUnit mUnits;
        bool mIsStopped;

    public:
        Stopwatch();
        ~Stopwatch();

        /**
         * @brief Sets the start time point of stopwatch
         */
        void Start() noexcept;

        /**
         * @brief Sets the end time point of stopwatch
         * @param[in] aCalculateAtStop - Flag to automatically calculate duration between points
         * when stopwatch stops. By default has **true** value.
         */
        void Stop(bool aCalculateAtStop = true) noexcept;

        /**
         * @brief Restarts stopwatch
         */
        void Restart() noexcept;

        /**
         * @brief Calculates duration between start and end point of stopwatch.
         * By default calls automaticaly when Stop() method called.
         */
        chrono::nanoseconds Duration();

        /**
         * @returns Returns TimeUnit value elapsed between points
         */
        const TimeUnit Elapsed();
    };
}

#endif // SOGE_UTILS_STOPWATCH_HPP
