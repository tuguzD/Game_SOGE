#ifndef SOGE_SOUND_SOUNDCOMPILEPREPROC_HPP
#define SOGE_SOUND_SOUNDCOMPILEPREPROC_HPP

#include "SOGE/Utils/PreprocessorHelpers.hpp"


namespace soge
{
    #define SOGE_REL_COMPILED_SOUNDSYSTEM_HEADER(Suffix) SOGE_REL_COMPILED_IMPL_HEADER_BASE(SOGE_INPUT_IMPL, Suffix)
    #define SOGE_ABS_COMPILED_SOUNDSYSTEM_HEADER(IncludeDir, Suffix)                                                \
        SOGE_ABS_COMPILED_IMPL_HEADER_BASE(SOGE_SOUND_IMPL, IncludeDir, Suffix)
}

#endif // SOGE_SOUND_SOUNDCOMPILEPREPROC_HPP
