#include "sogepch.hpp"

#include "SOGE/Graphics/Exceptions/NRIException.hpp"


namespace
{
    constexpr std::array g_exceptionWhat{
        "Render failure occurred",
        "Invalid argument were passed",
        "Out of memory",
        "Unsupported function",
        "Device has been lost",
        "Vulkan swapchain is out of date", // Documentation says this is Vulkan only
    };
}

namespace soge
{
    NRIException::NRIException(const nri::Result aNriResult) : m_nriResult(aNriResult)
    {
    }

    const char* NRIException::what() const noexcept
    {
        if (m_nriResult == nri::Result::SUCCESS || m_nriResult == nri::Result::MAX_NUM)
        {
            return nullptr;
        }

        const auto index = static_cast<std::uint8_t>(m_nriResult);
        return g_exceptionWhat[index + 1];
    }
}
