#include "sogepch.hpp"
#include "SOGE/Utils/Logger.hpp"
#include "SOGE/Utils/StackTrace.hpp"


namespace soge
{
    Logger::_LoggerPtr Logger::mEngineSideLogger;
    Logger::_LoggerPtr Logger::mApplicationSideLogger;

    void Logger::Init()
    {

        spdlog::set_pattern("%^[%T] %! [%n %l]: %v%$");

        mEngineSideLogger = _LoggerPtr(spdlog::stdout_color_mt("ENGINE").get());
        mEngineSideLogger->set_level(spdlog::level::trace);

        mApplicationSideLogger = _LoggerPtr(spdlog::stdout_color_mt("APP").get());
        mApplicationSideLogger->set_level(spdlog::level::trace);

        Logger::PrintStackTrace();
    }

    Logger::_LoggerRef Logger::GetEngineSideLogger()
    {
        return mEngineSideLogger;
    }

    Logger::_LoggerRef Logger::GetApplicationSideLogger()
    {
        return mApplicationSideLogger;
    }

    void Logger::PrintStackTrace()
    {
        auto formater = std::make_unique<spdlog::pattern_formatter>();
        formater->add_flag<StackTraceFormater>('*').set_pattern("%*");
        spdlog::set_formatter(std::move(formater));
        mEngineSideLogger->set_level(spdlog::level::critical);
        //mEngineSideLogger->set_formatter(std::move(formater));
    }

    //////////////////////////
    // StackTraceFormater
    /////////////////////////

    void StackTraceFormater::format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& aDest)
    {
        StackTrace stackTraceInfo;
        std::string stackTraceString = stackTraceInfo.Get();
        aDest.append(stackTraceString.data(), stackTraceString.data() + stackTraceString.size());
    }

    std::unique_ptr<spdlog::custom_flag_formatter> StackTraceFormater::clone() const
    {
        return spdlog::details::make_unique<StackTraceFormater>();
    }
}