#ifndef SOGE_GRAPHICS_EXCEPTIONS_NRIEXCEPTION_HPP
#define SOGE_GRAPHICS_EXCEPTIONS_NRIEXCEPTION_HPP

#include "SOGE/Utils/Logger.hpp"

#include <NRI.h>
#include <exception>


namespace soge
{
    class NRIException : public std::exception
    {
    private:
        nri::Result m_nriResult;

    public:
        explicit NRIException(nri::Result aNriResult);

        [[nodiscard]]
        const char* what() const noexcept override;
    };

    inline void NRIThrowIfFailed(const nri::Result aNriResult)
    {
        if (aNriResult == nri::Result::SUCCESS || aNriResult == nri::Result::MAX_NUM)
        {
            return;
        }

        NRIException exception(aNriResult);
        SOGE_ERROR_LOG("NRI Exception: {}", exception.what());
        throw std::move(exception);
    }

    inline void NRIThrowIfFailed(const nri::Result aNriResult, const eastl::string_view aContext)
    {
        if (aNriResult == nri::Result::SUCCESS || aNriResult == nri::Result::MAX_NUM)
        {
            return;
        }

        NRIException exception(aNriResult);
        SOGE_ERROR_LOG("NRI Exception: {} while {}", exception.what(), aContext.data());
        throw std::move(exception);
    }
}

#endif // SOGE_GRAPHICS_EXCEPTIONS_NRIEXCEPTION_HPP
