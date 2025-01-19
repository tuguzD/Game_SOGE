#include "sogepch.hpp"

#include "SOGE/Utils/Stopwatch.hpp"


namespace soge
{
    TimeUnit::TimeUnit() : m_nanoCount(0)
    {
    }

    TimeUnit::TimeUnit(chrono::nanoseconds aDuration)
    {
        m_nanoCount = chrono::duration_cast<chrono::nanoseconds>(aDuration).count();
    }

    TimeUnit::~TimeUnit()
    {
    }

    TimeUnit::ReturnType TimeUnit::Milliseconds() const
    {
        std::uint64_t up = ((m_nanoCount / 100000) % 10 >= 5) ? 1 : 0;
        return (m_nanoCount / 1000000) + up;
    }

    TimeUnit::ReturnType TimeUnit::Nanoseconds() const
    {
        return m_nanoCount;
    }

    TimeUnit::ReturnType TimeUnit::Microseconds() const
    {
        std::uint64_t up = ((m_nanoCount / 100) % 10 >= 5) ? 1 : 0;
        return (m_nanoCount / 1000) + up;
    }

    TimeUnit::ReturnType TimeUnit::Seconds() const
    {
        std::uint64_t up = ((m_nanoCount / 100000000) % 10 >= 5) ? 1 : 0;
        return (m_nanoCount / 1000000000) + up;
    }

    TimeUnit& TimeUnit::operator=(const TimeUnit& aOther)
    {
        m_nanoCount = aOther.m_nanoCount;
        return *this;
    }

    constexpr bool TimeUnit::operator==(const TimeUnit& aOther)
    {
        return m_nanoCount == aOther.m_nanoCount;
    }

    TimeUnit& TimeUnit::operator=(const chrono::nanoseconds& aNano)
    {
        m_nanoCount = chrono::duration_cast<chrono::nanoseconds>(aNano).count();
        return *this;
    }

    constexpr bool TimeUnit::operator==(const std::uint64_t& aValue)
    {
        return m_nanoCount == static_cast<std::uint64_t>(aValue);
    }

    Stopwatch::Stopwatch() : m_startTimePoint(), m_endTimePoint(), m_isStopped(false), m_units()
    {
    }

    Stopwatch::~Stopwatch()
    {
        if (!m_isStopped)
        {
            Stop();
        }
    }

    chrono::nanoseconds Stopwatch::Duration()
    {
        return m_endTimePoint - m_startTimePoint;
    }

    void Stopwatch::Start() noexcept
    {
        if (m_isStopped)
            m_isStopped = false;
        m_startTimePoint = ClockEngine::now();
    }

    void Stopwatch::Stop(bool aCalculateAtStop) noexcept
    {
        if (!m_isStopped)
        {
            m_endTimePoint = ClockEngine::now();
            m_isStopped = true;
        }

        if (aCalculateAtStop)
        {
            m_units = Duration();
        }
    }

    void Stopwatch::Restart() noexcept
    {
        Stop(false);
        Start();
    }

    TimeUnit Stopwatch::Elapsed()
    {
        return m_units;
    }
}
