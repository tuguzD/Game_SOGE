#ifndef SOGE_UTILS_LOGGER_HPP
#define SOGE_UTILS_LOGGER_HPP

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/null_sink.h>
#include <iostream>


namespace soge
{
    class Logger final
    {
        using _LoggerPtr = eastl::shared_ptr<spdlog::logger>;
        using _LoggerRef = _LoggerPtr&;

    private:
        static _LoggerPtr mEngineSideLogger;
        static _LoggerPtr mApplicationSideLogger;
        static bool mIsStackTraceOnErrorEnabled;
        static bool mIsStackTraceOnWarnEnabled;

        static void PrintStackTrace();

    public:
        static void Init();

        static _LoggerRef GetEngineSideLogger();
        static _LoggerRef GetApplicationSideLogger();

        template <typename... _Va>
        static void EngineLogErrorMessage(_Va&&... aArgs);

        template <typename... _Va>
        static void EngineLogWarnMessage(_Va&&... aArgs);

        template <typename... _Va>
        static void AppLogErrorMessage(_Va&&... aArgs);

        template <typename... _Va>
        static void AppLogWarnMessage(_Va&&... aArgs);
    };

    template <typename... _Va>
    inline void Logger::EngineLogErrorMessage(_Va&&... aArgs)
    {
        mEngineSideLogger->error(std::forward<_Va>(aArgs)...);
        if (mIsStackTraceOnErrorEnabled)
        {
            PrintStackTrace();
        }
    }

    template <typename... _Va>
    inline void Logger::EngineLogWarnMessage(_Va&&... aArgs)
    {
        mEngineSideLogger->warn(std::forward<_Va>(aArgs)...);
        if (mIsStackTraceOnWarnEnabled)
        {
            PrintStackTrace();
        }
    }

    template <typename... _Va>
    inline void Logger::AppLogErrorMessage(_Va&&... aArgs)
    {
        mApplicationSideLogger->error(std::forward<_Va>(aArgs)...);
        if (mIsStackTraceOnErrorEnabled)
        {
            PrintStackTrace();
        }
    }

    template <typename... _Va>
    inline void Logger::AppLogWarnMessage(_Va&&... aArgs)
    {
        mApplicationSideLogger->warn(std::forward<_Va>(aArgs)...);
        if (mIsStackTraceOnWarnEnabled)
        {
            PrintStackTrace();
        }
    }
}

#define SOGE_TRACE_LOG(...) ::soge::Logger::GetEngineSideLogger()->trace(__VA_ARGS__)
#define SOGE_INFO_LOG(...)  ::soge::Logger::GetEngineSideLogger()->info(__VA_ARGS__)
#define SOGE_WARN_LOG(...)  ::soge::Logger::EngineLogWarnMessage(__VA_ARGS__)
#define SOGE_ERROR_LOG(...) ::soge::Logger::EngineLogErrorMessage(__VA_ARGS__)

#define SOGE_APP_TRACE_LOG(...) ::soge::Logger::GetApplicationSideLogger()->trace(__VA_ARGS__)
#define SOGE_APP_INFO_LOG(...)  ::soge::Logger::GetApplicationSideLogger()->info(__VA_ARGS__)
#define SOGE_APP_WARN_LOG(...)  ::soge::Logger::AppLogWarnMessage(__VA_ARGS__)
#define SOGE_APP_ERROR_LOG(...) ::soge::Logger::AppLogErrorMessage(__VA_ARGS__)

#endif // SOGE_UTILS_LOGGER_HPP
