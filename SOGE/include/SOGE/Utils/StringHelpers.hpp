#ifndef SOGE_UTILS_STRINGHELPERS_HPP
#define SOGE_UTILS_STRINGHELPERS_HPP


namespace soge
{
    std::wstring StdToWide(const std::string& narrow);
    std::string StdToNarrow(const std::wstring& wide);
}

#endif // SOGE_UTILS_STRINGHELPERS_HPP
