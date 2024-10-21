#include "sogepch.hpp"

#include "SOGE/Utils/StringHelpers.hpp"


namespace soge
{
    std::wstring StdToWide(const std::string& aNarrow)
    {
        std::wstring wide;
        wide.resize(aNarrow.size() + 1);

        std::size_t actual;
        mbstowcs_s(&actual, wide.data(), wide.size(), aNarrow.c_str(), _TRUNCATE);
        if (actual > 0)
        {
            wide.resize(actual - 1);
            return wide;
        }

        return {};
    }

    std::string StdToNarrow(const std::wstring& aWide)
    {
        // Wide string has 2 bytes per character
        // Narrow string has variable bytes per character
        // So we double the size to match sizes
        // Actually it may have larger size, so if it will be a problem, try increase the size

        std::string narrow;
        narrow.resize(aWide.size() * 2);

        std::size_t actual;
        wcstombs_s(&actual, narrow.data(), narrow.size(), aWide.c_str(), _TRUNCATE);
        narrow.resize(actual - 1);
        return narrow;
    }
}
