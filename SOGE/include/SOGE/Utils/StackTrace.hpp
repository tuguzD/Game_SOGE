#ifndef SOGE_UTILS_STACKTRACE_HPP
#define SOGE_UTILS_STACKTRACE_HPP


namespace backward
{
    class StackTrace;
}

namespace soge
{
    class StackTrace
    {
    private:
        // Using Pimpl technique to prevent a lot of backward::stacktrace including
        std::unique_ptr<backward::StackTrace> m_stackTrace;

    public:
        StackTrace();
        StackTrace(const StackTrace& aOther);
        ~StackTrace();

        [[nodiscard]]
        std::wstring GetWide() const;
        [[nodiscard]]
        std::string Get() const;

        StackTrace& operator=(const StackTrace& aOther);
    };
}

#endif // SOGE_UTILS_STACKTRACE_HPP
