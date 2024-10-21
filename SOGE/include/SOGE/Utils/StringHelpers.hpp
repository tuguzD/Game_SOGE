#ifndef SOGE_UTILS_STRINGHELPERS_HPP
#define SOGE_UTILS_STRINGHELPERS_HPP


namespace soge
{
    std::wstring StdToWide(const std::string& aNarrow);
    std::string StdToNarrow(const std::wstring& aWide);
}

#endif // SOGE_UTILS_STRINGHELPERS_HPP
