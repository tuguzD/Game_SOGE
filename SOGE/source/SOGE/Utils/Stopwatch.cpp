#include "sogepch.hpp"

#include "SOGE/Utils/Stopwatch.hpp"


namespace soge
{
    TimeUnit::TimeUnit() : mNanoCount(0)
    {
    }

    TimeUnit::TimeUnit(chrono::nanoseconds aDuration)
    {
        mNanoCount = chrono::duration_cast<chrono::nanoseconds>(aDuration).count();
    }

    TimeUnit::~TimeUnit()
    {
    }

    const TimeUnit::ReturnType TimeUnit::Milliseconds() const
    {
        std::uint64_t up = ((mNanoCount / 100000) % 10 >= 5) ? 1 : 0;
        return (mNanoCount / 1000000) + up;
    }

    const TimeUnit::ReturnType TimeUnit::Nanoseconds() const
    {
        return mNanoCount;
    }

    const TimeUnit::ReturnType TimeUnit::Microseconds() const
    {
        std::uint64_t up = ((mNanoCount / 100) % 10 >= 5) ? 1 : 0;
        return (mNanoCount / 1000) + up;
    }

    const TimeUnit::ReturnType TimeUnit::Seconds() const
    {
        std::uint64_t up = ((mNanoCount / 100000000) % 10 >= 5) ? 1 : 0;
        return (mNanoCount / 1000000000) + up;
    }

    TimeUnit& TimeUnit::operator=(const TimeUnit& aOther)
    {
        mNanoCount = aOther.mNanoCount;
        return *this;
    }

    constexpr bool TimeUnit::operator==(const TimeUnit& aOther)
    {
        return mNanoCount == aOther.mNanoCount;
    }

    TimeUnit& TimeUnit::operator=(const chrono::nanoseconds& aNano)
    {
        mNanoCount = chrono::duration_cast<chrono::nanoseconds>(aNano).count();
        return *this;
    }

    constexpr bool TimeUnit::operator==(const std::uint64_t& aValue)
    {
        return mNanoCount == static_cast<std::uint64_t>(aValue);
    }

    Stopwatch::Stopwatch() : mStartTimePoint(), mEndTimePoint(), mIsStopped(false), mUnits()
    {
    }

    Stopwatch::~Stopwatch()
    {
        if (!mIsStopped)
        {
            Stop();
        }
    }

    chrono::nanoseconds Stopwatch::Duration()
    {
        return mEndTimePoint - mStartTimePoint;
    }

    void Stopwatch::Start() noexcept
    {
        if (mIsStopped)
            mIsStopped = false;
        mStartTimePoint = _ClockEngine::now();
    }

    void Stopwatch::Stop(bool aCalculateAtStop) noexcept
    {
        if (!mIsStopped)
        {
            mEndTimePoint = _ClockEngine::now();
            mIsStopped = true;
        }

        if (aCalculateAtStop)
        {
            mUnits = Duration();
        }
    }

    void Stopwatch::Restart() noexcept
    {
        Stop(false);
        Start();
    }

    const TimeUnit Stopwatch::Elapsed()
    {
        return mUnits;
    }
}
