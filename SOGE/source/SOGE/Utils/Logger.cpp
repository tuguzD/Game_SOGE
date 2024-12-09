#include "sogepch.hpp"

#include "SOGE/Utils/Logger.hpp"
#include "SOGE/Utils/StackTrace.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>


namespace soge
{
    Logger::LoggerPtr Logger::s_engineSideLogger;
    Logger::LoggerPtr Logger::s_applicationSideLogger;

    bool Logger::s_isStackTraceOnErrorEnabled = true;
    bool Logger::s_isStackTraceOnWarnEnabled = false;

    void Logger::Init()
    {
        spdlog::set_pattern("%^[%T] %! [%n %l]: %v%$");

        s_engineSideLogger = spdlog::stdout_color_mt("ENGINE");
        s_engineSideLogger->set_level(spdlog::level::trace);

        s_applicationSideLogger = spdlog::stdout_color_mt("APP");
        s_applicationSideLogger->set_level(spdlog::level::trace);
    }

    auto Logger::GetEngineSideLogger() -> LoggerPtr
    {
        return s_engineSideLogger;
    }

    auto Logger::GetApplicationSideLogger() -> LoggerPtr
    {
        return s_applicationSideLogger;
    }

    void Logger::PrintStackTrace()
    {
        if (s_isStackTraceOnErrorEnabled && s_engineSideLogger != nullptr)
        {
            const StackTrace stackTraceInfo{};
            s_engineSideLogger->debug(stackTraceInfo.Get());
        }
    }
}
