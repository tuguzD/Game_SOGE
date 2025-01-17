#ifndef SOGE_UTILS_STRINGHELPERS_HPP
#define SOGE_UTILS_STRINGHELPERS_HPP


namespace soge
{
    std::wstring StdToWide(std::string_view aNarrow);
    std::string StdToNarrow(std::wstring_view aWide);

    std::string EANarrowToStd(const eastl::string& aNarrow);
    std::wstring EAWideToStd(const eastl::wstring& aWide);

    eastl::wstring EAToWide(eastl::string_view aNarrow);
    eastl::string EAToNarrow(eastl::wstring_view aWide);


}

#endif // SOGE_UTILS_STRINGHELPERS_HPP
