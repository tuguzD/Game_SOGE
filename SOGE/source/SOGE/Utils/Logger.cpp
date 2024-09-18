#include "sogepch.hpp"
#include "SOGE/Utils/Logger.hpp"


namespace soge
{
    Logger::_LoggerPtr Logger::mEngineSideLogger;
    Logger::_LoggerPtr Logger::mApplicationSideLogger;

    void Logger::Init()
    {
        spdlog::set_pattern("%^[%T] [%n %l]: %v%$");

        mEngineSideLogger = _LoggerPtr(spdlog::stdout_color_mt("ENGINE").get());
        mEngineSideLogger->set_level(spdlog::level::trace);

        mApplicationSideLogger = _LoggerPtr(spdlog::stdout_color_mt("APP").get());
        mApplicationSideLogger->set_level(spdlog::level::trace);

    }

    Logger::_LoggerRef Logger::GetEngineSideLogger()
    {
        return mEngineSideLogger;
    }

    Logger::_LoggerRef Logger::GetApplicationSideLogger()
    {
        return mApplicationSideLogger;
    }
}