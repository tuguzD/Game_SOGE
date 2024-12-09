#ifndef SOGE_UTILS_PREPROCESSORHELPERS_HPP
#define SOGE_UTILS_PREPROCESSORHELPERS_HPP


///////////////////////////////////////
// HELPERS FROM UNREAL ENGINE REPO
// https://shorturl.at/zxOyf
//////////////////////////////////////

// Turns a preprocessor token into a real string
#define UE_STRINGIZE(Token)         UE_PRIVATE_STRINGAZE(Token)
#define UE_PRIVATE_STRINGAZE(Token) #Token

// Concatenates two preprocessor tokens, performing macro expansion on them first
#define UE_JOIN(TokenA, TokenB)         UE_PRIVATE_JOIN(TokenA, TokenB)
#define UE_PRIVATE_JOIN(TokenA, TokenB) TokenA##TokenB

// setup standardized way of including platform headers from the "uber-platform" headers like PlatformFile.h
#ifdef OVERRIDE_PLATFORM_HEADER_NAME
// allow for an override, so compiled platforms Win64 and Win32 will both include Windows
#define PLATFORM_HEADER_NAME OVERRIDE_PLATFORM_HEADER_NAME
#else
// otherwise use the compiled platform name
#define PLATFORM_HEADER_NAME SOGE_COMPILED_PLATFORM
#endif

///////////////////////////////////
/// END OF UNREAL ENGINE HELPERS
//////////////////////////////////

// clang-format off
#define SOGE_REL_COMPILED_IMPL_HEADER_BASE(Libname, Suffix)                                                             \
    UE_STRINGIZE(UE_JOIN(Impl/Libname/Libname, Suffix))
#define SOGE_ABS_COMPILED_IMPL_HEADER_BASE(Libname, IncludeDir, Suffix)                                                 \
    UE_STRINGIZE(UE_JOIN(IncludeDir, /UE_JOIN(Impl/Libname/Libname, Suffix)))

#define REL_COMPILED_PLATFORM_HEADER(Suffix)                                                                           \
    UE_STRINGIZE(UE_JOIN(Platform/PLATFORM_HEADER_NAME/PLATFORM_HEADER_NAME, Suffix))
#define ABS_COMPILED_PLATFORM_HEADER(IncludeDir, Suffix)                                                               \
    UE_STRINGIZE(UE_JOIN(IncludeDir, /UE_JOIN(PLATFORM_HEADER_NAME, Suffix)))

//#define SG_REL_COMPILED_IMPL_HEADER(Suffix)                                                                            \
//    UE_STRINGIZE(UE_JOIN(Impl/SOGE_INPUT_IMPL/SOGE_INPUT_IMPL, Suffix))
//#define SG_ABS_COMPILED_IMPL_HEADER(IncludeDir, Suffix)                                                                \
//    UE_STRINGIZE(UE_JOIN(IncludeDir, /UE_JOIN(Impl/SOGE_INPUT_IMPL/SOGE_INPUT_IMPL, Suffix)))
// clang-format on

#endif // SOGE_UTILS_PREPROCESSORHELPERS_HPP
