#include "sogepch.hpp"
#include "SOGE/Graphics/Exceptions/NRIException.hpp"


namespace soge
{
    NRIException::NRIException(nri::Result aNriResult) : m_nriResult(aNriResult)
    {
        m_exceptionInfo[0] = "Render failure occurred";
        m_exceptionInfo[1] = "Invalid argument were passed";
        m_exceptionInfo[2] = "Out of memory";
        m_exceptionInfo[3] = "Unsupported function";
        m_exceptionInfo[4] = "Device has been lost";
        m_exceptionInfo[5] = "Vulkan swapchain is out of date"; // Documentation says it's Vulkan only
    }

    const char* NRIException::what() const noexcept
    {
        unsigned int num = static_cast<unsigned int>(m_nriResult);
        if (num != 0 && num != 7) // Just in case this used outside of NRIThrowIfFailed
            return m_exceptionInfo[num + 1];
    }
}
