#ifndef SOGE_UTILS_LOGGER_HPP
#define SOGE_UTILS_LOGGER_HPP

#include <spdlog/logger.h>


namespace soge
{
    class Logger final
    {
    public:
        template <typename... Args>
        using FmtString = spdlog::format_string_t<Args...>;

        using Level = spdlog::level::level_enum;

    private:
        using LoggerPtr = std::shared_ptr<spdlog::logger>;
        using LoggerRef = LoggerPtr&;

        static LoggerPtr s_engineSideLogger;
        static LoggerPtr s_applicationSideLogger;

        static bool s_isStackTraceOnErrorEnabled;
        static bool s_isStackTraceOnWarnEnabled;

        template <typename... Args>
        static void Log(LoggerPtr& aLogger, Level aLevel, bool aPrintStackTrace, FmtString<Args...> aFmt,
                        Args&&... args);

        static void PrintStackTrace();

    public:
        explicit Logger() = delete;

        static void Init();

        static LoggerPtr GetEngineSideLogger();
        static LoggerPtr GetApplicationSideLogger();

        template <typename... Args>
        static void EngineLog(Level aLevel, FmtString<Args...> aFmt, Args&&... args);

        template <typename... Args>
        static void EngineError(FmtString<Args...> aFmt, Args&&... args);

        template <typename... Args>
        static void EngineWarn(FmtString<Args...> aFmt, Args&&... args);

        template <typename... Args>
        static void EngineInfo(FmtString<Args...> aFmt, Args&&... args);

        template <typename... Args>
        static void EngineTrace(FmtString<Args...> aFmt, Args&&... args);

        template <typename... Args>
        static void AppLog(Level aLevel, FmtString<Args...> aFmt, Args&&... args);

        template <typename... Args>
        static void AppError(FmtString<Args...> aFmt, Args&&... args);

        template <typename... Args>
        static void AppWarn(FmtString<Args...> aFmt, Args&&... args);

        template <typename... Args>
        static void AppInfo(FmtString<Args...> aFmt, Args&&... args);

        template <typename... Args>
        static void AppTrace(FmtString<Args...> aFmt, Args&&... args);
    };

    template <typename... Args>
    void Logger::Log(LoggerPtr& aLogger, Level aLevel, const bool aPrintStackTrace, FmtString<Args...> aFmt,
                     Args&&... args)
    {
        if (aLogger == nullptr)
        {
            return;
        }

        aLogger->log(aLevel, aFmt, std::forward<Args>(args)...);
        if (aPrintStackTrace)
        {
            PrintStackTrace();
        }
    }

    template <typename... Args>
    void Logger::EngineLog(Level aLevel, FmtString<Args...> aFmt, Args&&... args)
    {
        bool printStackTrace = (aLevel == Level::err && s_isStackTraceOnErrorEnabled) ||
                               (aLevel == Level::warn && s_isStackTraceOnWarnEnabled);
        Log(s_engineSideLogger, aLevel, printStackTrace, aFmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Logger::EngineError(FmtString<Args...> aFmt, Args&&... args)
    {
        Log(s_engineSideLogger, Level::err, s_isStackTraceOnErrorEnabled, aFmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Logger::EngineWarn(FmtString<Args...> aFmt, Args&&... args)
    {
        Log(s_engineSideLogger, Level::warn, s_isStackTraceOnWarnEnabled, aFmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Logger::EngineInfo(FmtString<Args...> aFmt, Args&&... args)
    {
        Log(s_engineSideLogger, Level::info, false, aFmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Logger::EngineTrace(FmtString<Args...> aFmt, Args&&... args)
    {
        Log(s_engineSideLogger, Level::trace, false, aFmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Logger::AppLog(Level aLevel, FmtString<Args...> aFmt, Args&&... args)
    {
        bool printStackTrace = (aLevel == Level::err && s_isStackTraceOnErrorEnabled) ||
                               (aLevel == Level::warn && s_isStackTraceOnWarnEnabled);
        Log(s_applicationSideLogger, aLevel, printStackTrace, aFmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Logger::AppError(FmtString<Args...> aFmt, Args&&... args)
    {
        Log(s_applicationSideLogger, Level::err, s_isStackTraceOnErrorEnabled, aFmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Logger::AppWarn(FmtString<Args...> aFmt, Args&&... args)
    {
        Log(s_applicationSideLogger, Level::warn, s_isStackTraceOnWarnEnabled, aFmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Logger::AppInfo(FmtString<Args...> aFmt, Args&&... args)
    {
        Log(s_applicationSideLogger, Level::info, false, aFmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Logger::AppTrace(FmtString<Args...> aFmt, Args&&... args)
    {
        Log(s_applicationSideLogger, Level::trace, false, aFmt, std::forward<Args>(args)...);
    }
}

#define SOGE_TRACE_LOG(...) ::soge::Logger::EngineTrace(__VA_ARGS__)
#define SOGE_INFO_LOG(...)  ::soge::Logger::EngineInfo(__VA_ARGS__)
#define SOGE_WARN_LOG(...)  ::soge::Logger::EngineWarn(__VA_ARGS__)
#define SOGE_ERROR_LOG(...) ::soge::Logger::EngineError(__VA_ARGS__)

#define SOGE_APP_TRACE_LOG(...) ::soge::Logger::AppTrace(__VA_ARGS__)
#define SOGE_APP_INFO_LOG(...)  ::soge::Logger::AppInfo(__VA_ARGS__)
#define SOGE_APP_WARN_LOG(...)  ::soge::Logger::AppWarn(__VA_ARGS__)
#define SOGE_APP_ERROR_LOG(...) ::soge::Logger::AppError(__VA_ARGS__)

#endif // SOGE_UTILS_LOGGER_HPP
