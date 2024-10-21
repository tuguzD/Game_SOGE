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
        using LoggerPtr = eastl::shared_ptr<spdlog::logger>;
        using LoggerRef = LoggerPtr&;

    private:
        static LoggerPtr s_engineSideLogger;
        static LoggerPtr s_applicationSideLogger;
        static bool s_isStackTraceOnErrorEnabled;
        static bool s_isStackTraceOnWarnEnabled;

        static void PrintStackTrace();

    public:
        static void Init();

        static LoggerRef GetEngineSideLogger();
        static LoggerRef GetApplicationSideLogger();

        template <typename... Args>
        static void EngineLogErrorMessage(Args&&... args);

        template <typename... Args>
        static void EngineLogWarnMessage(Args&&... args);

        template <typename... Args>
        static void AppLogErrorMessage(Args&&... args);

        template <typename... Args>
        static void AppLogWarnMessage(Args&&... args);
    };

    template <typename... Args>
    inline void Logger::EngineLogErrorMessage(Args&&... args)
    {
        s_engineSideLogger->error(std::forward<Args>(args)...);
        if (s_isStackTraceOnErrorEnabled)
        {
            PrintStackTrace();
        }
    }

    template <typename... Args>
    inline void Logger::EngineLogWarnMessage(Args&&... args)
    {
        s_engineSideLogger->warn(std::forward<Args>(args)...);
        if (s_isStackTraceOnWarnEnabled)
        {
            PrintStackTrace();
        }
    }

    template <typename... Args>
    inline void Logger::AppLogErrorMessage(Args&&... args)
    {
        s_applicationSideLogger->error(std::forward<Args>(args)...);
        if (s_isStackTraceOnErrorEnabled)
        {
            PrintStackTrace();
        }
    }

    template <typename... Args>
    inline void Logger::AppLogWarnMessage(Args&&... args)
    {
        s_applicationSideLogger->warn(std::forward<Args>(args)...);
        if (s_isStackTraceOnWarnEnabled)
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
