#ifndef SOGE_STACK_TRACE_HPP
#define SOGE_STACK_TRACE_HPP


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
        std::unique_ptr<backward::StackTrace> mStackTrace;

    public:
        StackTrace();
        StackTrace(const StackTrace& aOther);
        ~StackTrace();

        std::wstring GetWide() const;
        std::string Get() const;

        StackTrace& operator=(const StackTrace& aOther);

    };
}

#endif // !SOGE_STACK_TRACE_HPP
