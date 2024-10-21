#include "sogepch.hpp"

#include "SOGE/Utils/StackTrace.hpp"
#include "SOGE/Utils/StringHelpers.hpp"

#include <backward.hpp>


namespace soge
{
    StackTrace::StackTrace()
    {
        backward::TraceResolver workAround; // https://github.com/bombela/backward-cpp/issues/206
        m_stackTrace = std::make_unique<backward::StackTrace>();
        m_stackTrace->load_here(64);
        m_stackTrace->skip_n_firsts(3);
    }

    StackTrace::StackTrace(const StackTrace& aOther)
        : m_stackTrace(std::make_unique<backward::StackTrace>(*m_stackTrace))
    {
    }

    StackTrace::~StackTrace()
    {
    }

    std::wstring StackTrace::GetWide() const
    {
        std::ostringstream oss;
        backward::Printer printer;

        printer.print(*m_stackTrace, oss);
        return StdToWide(oss.str());
    }

    std::string StackTrace::Get() const
    {
        std::ostringstream oss;
        backward::Printer printer;

        printer.print(*m_stackTrace, oss);
        return oss.str();
    }

    StackTrace& StackTrace::operator=(const StackTrace& aOther)
    {
        m_stackTrace = std::make_unique<backward::StackTrace>(*m_stackTrace);
        return *this;
    }
}
