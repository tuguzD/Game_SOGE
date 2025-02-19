#ifndef SOGE_UTILS_PREPROCESSORHELPERS_HPP
#define SOGE_UTILS_PREPROCESSORHELPERS_HPP


///////////////////////////////////////
// HELPERS FROM UNREAL ENGINE REPO
// https://shorturl.at/zxOyf
//////////////////////////////////////

// Turns a preprocessor token into a real string
#define SOGE_STRINGIZE(Token)         SOGE_PRIVATE_STRINGAZE(Token)
#define SOGE_PRIVATE_STRINGAZE(Token) #Token

// Concatenates two preprocessor tokens, performing macro expansion on them first
#define SOGE_JOIN(TokenA, TokenB)         SOGE_PRIVATE_JOIN(TokenA, TokenB)
#define SOGE_PRIVATE_JOIN(TokenA, TokenB) TokenA##TokenB

// setup standardized way of including platform headers from the "uber-platform" headers like PlatformFile.h
#ifdef OVERRIDE_PLATFORM_HEADER_NAME
// allow for an override, so compiled platforms Win64 and Win32 will both include Windows
#define PLATFORM_HEADER_NAME OVERRIDE_PLATFORM_HEADER_NAME
#else
// otherwise use the compiled platform name
#define PLATFORM_HEADER_NAME SOGE_COMPILED_PLATFORM
#endif

///////////////////////////////////
/// END OF HELPERS FROM UNREAL ENGINE
//////////////////////////////////

#define SOGE_CONCAT_MACRO_AND_NAME(x, y) x##y
#define SOGE_CONCAT_MACRO(x, y) SOGE_CONCAT_MACRO_AND_NAME(x, y)

// NOLINTBEGIN(bugprone-macro-parentheses)
#define SOGE_REL_COMPILED_IMPL_HEADER(LibName, Suffix) SOGE_STRINGIZE(SOGE_JOIN(Impl/LibName/LibName, Suffix))
#define SOGE_ABS_COMPILED_IMPL_HEADER(LibName, IncludeDir, Suffix)                                                     \
    SOGE_STRINGIZE(SOGE_JOIN(IncludeDir, /SOGE_JOIN(Impl/LibName/LibName, Suffix)))
#define SOGE_MAKE_IMPL_CLASS_FRIEND(LibName, ClassName) friend class SOGE_CONCAT_MACRO(LibName, ClassName)
// NOLINTEND(bugprone-macro-parentheses)

#define SOGE_REL_COMPILED_PLATFORM_HEADER(Suffix)                                                                      \
    SOGE_STRINGIZE(SOGE_JOIN(Platform/PLATFORM_HEADER_NAME/PLATFORM_HEADER_NAME, Suffix))
#define SOGE_ABS_COMPILED_PLATFORM_HEADER(IncludeDir, Suffix)                                                          \
    SOGE_STRINGIZE(SOGE_JOIN(IncludeDir, /SOGE_JOIN(PLATFORM_HEADER_NAME, Suffix)))

#define SOGE_LOG_TITLE_DIVIDER(title)                                                                                  \
    SOGE_INFO_LOG("\n\n--------------------\n {} \n--------------------\n\n", SOGE_STRINGIZE(title));

#endif // SOGE_UTILS_PREPROCESSORHELPERS_HPP
