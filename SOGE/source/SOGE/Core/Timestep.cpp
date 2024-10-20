#include "sogepch.hpp"

#include "SOGE/Core/Timestep.hpp"


namespace soge
{
    std::uint64_t Timestep::mFrameTime = 0;
    std::uint64_t Timestep::mFrameTimeForRealDelta = 0;
    float Timestep::mDeltaTime = 0.0f;
    float Timestep::mRealDeltaTime = 0.0f;
    float Timestep::mGlobalTime = 0.0f;


    void Timestep::StartFrame()
    {
        if (mFrameTime == 0)
        {
            mFrameTime = GetMicroseconds();
            mFrameTimeForRealDelta = mFrameTime;
        }

        std::uint64_t timestamp = GetMicroseconds();
        float delta = static_cast<float>((timestamp - mFrameTime) / 1000000.0f);

        mRealDeltaTime = delta;
        mDeltaTime = eastl::min(0.1f, eastl::max(0.0004f, delta));

        mFrameTime = timestamp;
    }

    void Timestep::CalculateDelta()
    {
        mRealDeltaTime = static_cast<float>((mFrameTime - mFrameTimeForRealDelta) / 1000000.0f);
        mFrameTimeForRealDelta = mFrameTime;
    }

    void Timestep::SetDelta(float aDelta)
    {
        mDeltaTime = aDelta;
    }

    float Timestep::DeltaTime()
    {
        return mDeltaTime;
    }

    float Timestep::RealDeltaTime()
    {
        return mRealDeltaTime;
    }

    float Timestep::FrameTime()
    {
        return mFrameTime;
    }

    std::uint64_t Timestep::GetMilliseconds()
    {
        return _ChronoNSpace::duration_cast<_ChronoNSpace::milliseconds>(_ClockEngine::now().time_since_epoch())
            .count();
    }

    std::uint64_t Timestep::GetMicroseconds()
    {
        return _ChronoNSpace::duration_cast<_ChronoNSpace::microseconds>(_ClockEngine::now().time_since_epoch())
            .count();
    }

    std::uint64_t Timestep::GetNanoseconds()
    {
        return _ChronoNSpace::duration_cast<_ChronoNSpace::nanoseconds>(_ClockEngine::now().time_since_epoch()).count();
    }

    std::uint64_t Timestep::GetSeconds()
    {
        return _ChronoNSpace::duration_cast<_ChronoNSpace::seconds>(_ClockEngine::now().time_since_epoch()).count();
    }
}
