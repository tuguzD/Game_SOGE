#ifndef SOGE_GRAPHICS_EXCEPTIONS_NRIEXCEPTION_HPP
#define SOGE_GRAPHICS_EXCEPTIONS_NRIEXCEPTION_HPP

#include <exception>
#include "SOGE/Utils/Logger.hpp"
#include <NRI.h>

namespace soge
{
    class NRIException : public std::exception
    {
    private:
        // Only for error info
        const char* m_exceptionInfo[6];
        nri::Result m_nriResult;

    public:
        NRIException(nri::Result aNriResult);
        ~NRIException() = default;
        const char* what() const noexcept override;

    };

    inline void NRIThrowIfFailed(nri::Result aNriResult)
    {
        auto except = NRIException(aNriResult);
        unsigned int num = static_cast<unsigned int>(aNriResult);
        if (num != 0 && num != 7)
        {
            SOGE_ERROR_LOG("NRI Exception: {}", except.what());
            throw except;
        }
    };

    inline void NRIThrowIfFailed(nri::Result aNriResult, const eastl::string_view aContext)
    {
        auto except = NRIException(aNriResult);
        unsigned int num = static_cast<unsigned int>(aNriResult);
        if (num != 0 && num != 7)
        {
            SOGE_ERROR_LOG("NRI Exception: {0} while {1}", except.what(), aContext.data());
            throw except;
        }
    };
}

#endif // SOGE_GRAPHICS_EXCEPTIONS_NRIEXCEPTION_HPP
