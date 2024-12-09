#ifndef SOGE_UTILS_STRINGHELPERS_HPP
#define SOGE_UTILS_STRINGHELPERS_HPP


namespace soge
{
    std::wstring StdToWide(const std::string& aNarrow);
    std::string StdToNarrow(const std::wstring& aWide);

    std::string EANarrowToStd(const eastl::string& aNarrow);
    std::wstring EAWideToStd(const eastl::wstring& aWide);

    eastl::wstring EAToWide(const eastl::string& aNarrow);
    eastl::string EAToNarrow(const eastl::wstring& aWide);


}

#endif // SOGE_UTILS_STRINGHELPERS_HPP
