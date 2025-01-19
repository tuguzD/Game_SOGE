#ifndef SOGE_SOUND_SOUNDCOMPILEPREPROC_HPP
#define SOGE_SOUND_SOUNDCOMPILEPREPROC_HPP

#include "SOGE/Utils/PreprocessorHelpers.hpp"


#define SOGE_REL_COMPILED_SOUNDSYSTEM_IMPL_HEADER(Suffix) SOGE_REL_COMPILED_IMPL_HEADER(SOGE_SOUND_IMPL, Suffix)
#define SOGE_ABS_COMPILED_SOUNDSYSTEM_IMPL_HEADER(IncludeDir, Suffix)                                         \
    SOGE_ABS_COMPILED_IMPL_HEADER(SOGE_SOUND_IMPL, IncludeDir, Suffix)
#define SOGE_MAKE_SOUNDSYSTEM_CLASS_FRIEND(ClassName)                                                           \
    SOGE_MAKE_IMPL_CLASS_FRIEND(SOGE_SOUND_IMPL, ClassName);

#endif // SOGE_SOUND_SOUNDCOMPILEPREPROC_HPP
