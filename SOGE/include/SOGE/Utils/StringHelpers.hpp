#ifndef SOGE_STRING_HELPERS_HPP
#define SOGE_STRING_HELPERS_HPP


namespace soge
{
    std::wstring StdToWide(const std::string& aNarrow);
    std::string StdToNarrow(const std::wstring& aWide);
}

#endif // !SOGE_STRING_HELPERS_HPP
