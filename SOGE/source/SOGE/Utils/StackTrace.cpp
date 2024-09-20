#include "sogepch.hpp"
#include "SOGE/Utils/StackTrace.hpp"
#include "SOGE/Utils/StringHelpers.hpp"

#include <backward.hpp>


namespace soge
{
    StackTrace::StackTrace()
    {
        backward::TraceResolver workAround; // https://github.com/bombela/backward-cpp/issues/206
        mStackTrace = std::make_unique<backward::StackTrace>();
        mStackTrace->load_here(64);
        mStackTrace->skip_n_firsts(10);
    }

    StackTrace::StackTrace(const StackTrace& aOther)
        : mStackTrace(std::make_unique<backward::StackTrace>(*mStackTrace))
    {
    }

    StackTrace::~StackTrace()
    {
    }

    std::wstring StackTrace::GetWide() const
    {
        std::ostringstream oss;
        backward::Printer printer;

        printer.print(*mStackTrace, oss);
        return StdToWide(oss.str());
    }

    std::string StackTrace::Get() const
    {
        std::ostringstream oss;
        backward::Printer printer;

        printer.print(*mStackTrace, oss);
        return oss.str();
    }

    StackTrace& StackTrace::operator=(const StackTrace& aOther)
    {
        mStackTrace = std::make_unique<backward::StackTrace>(*mStackTrace);
        return *this;
    }
}