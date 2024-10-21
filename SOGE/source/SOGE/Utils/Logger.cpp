#include "sogepch.hpp"

#include "SOGE/Utils/Logger.hpp"
#include "SOGE/Utils/StackTrace.hpp"


namespace soge
{
    Logger::LoggerPtr Logger::s_engineSideLogger;
    Logger::LoggerPtr Logger::s_applicationSideLogger;
    bool Logger::s_isStackTraceOnErrorEnabled = true;
    bool Logger::s_isStackTraceOnWarnEnabled = false;

    void Logger::Init()
    {

        spdlog::set_pattern("%^[%T] %! [%n %l]: %v%$");

        s_engineSideLogger = LoggerPtr(spdlog::stdout_color_mt("ENGINE").get());
        s_engineSideLogger->set_level(spdlog::level::trace);

        s_applicationSideLogger = LoggerPtr(spdlog::stdout_color_mt("APP").get());
        s_applicationSideLogger->set_level(spdlog::level::trace);
    }

    Logger::LoggerRef Logger::GetEngineSideLogger()
    {
        return s_engineSideLogger;
    }

    Logger::LoggerRef Logger::GetApplicationSideLogger()
    {
        return s_applicationSideLogger;
    }

    void Logger::PrintStackTrace()
    {
        if (s_isStackTraceOnErrorEnabled)
        {
            StackTrace stackTraceInfo;
            s_engineSideLogger->debug(stackTraceInfo.Get());
        }
    }
}
