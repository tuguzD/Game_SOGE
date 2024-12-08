
-- Include directories
IncludeThirdpartyDirs = {}
IncludeThirdpartyDirs["spdlog"] = "3rdparty/spdlog/spdlog/include"
IncludeThirdpartyDirs["backwardcpp"] = "3rdparty/backwardcpp/backwardcpp"
IncludeThirdpartyDirs["EASTL"] = "3rdparty/EASTL/EASTL/include"
IncludeThirdpartyDirs["EABase"] = "3rdparty/EASTL/EASTL/deps/EABase/include/Common/"
IncludeThirdpartyDirs["UUID_v4"] = "3rdparty/uuid_v4/uuid_v4/include"
IncludeThirdpartyDirs["eventpp"] = "3rdparty/eventpp/eventpp/include"
IncludeThirdpartyDirs["kangaru"] = "3rdparty/kangaru/kangaru/include"
IncludeThirdpartyDirs["SDL3"] = "3rdparty/SDL/SDL/include"
IncludeThirdpartyDirs["FMOD_CORE"] = "3rdparty/FMOD/FMOD/core/inc"
IncludeThirdpartyDirs["FMOD_FSBANK"] = "3rdparty/FMOD/FMOD/fsbank/inc"
IncludeThirdpartyDirs["FMOD_STUDIO"] = "3rdparty/FMOD/FMOD/studio/inc"

-- Include libs
Libraries = {}

-- SDL Libs

Libraries["SDL_UCLIB_D"] = "3rdparty/SDL/lib/Debug/SDL_uclibc.lib"
Libraries["SDL3_DLL_D"] = "3rdparty/SDL/lib/Debug/SDL3.dll"
Libraries["SDL3_LIB_D"] = "3rdparty/SDL/lib/Debug/SDL3.lib"

Libraries["SDL_UCLIB_R"] = "3rdparty/SDL/lib/Release/SDL_uclibc.lib"
Libraries["SDL3_DLL_R"] = "3rdparty/SDL/lib/Release/SDL3.dll"
Libraries["SDL3_LIB_R"] = "3rdparty/SDL/lib/Release/SDL3.lib"

-- FMOD Libs

Libraries["FMOD_WIN64_CORE_DLL_D"] = "3rdparty/FMOD/FMOD/core/lib/x64/fmodL.dll"
Libraries["FMOD_WIN64_CORE_D"] = "3rdparty/FMOD/FMOD/core/lib/x64/fmodL_vc.lib"

Libraries["FMOD_WIN64_CORE_DLL_R"] = "3rdparty/FMOD/FMOD/core/lib/x64/fmod.dll"
Libraries["FMOD_WIN64_CORE_R"] = "3rdparty/FMOD/FMOD/core/lib/x64/fmod_vc.lib"

-- No debug\release separation
Libraries["FMOD_WIN64_FSBANK_DLL"] = "3rdparty/FMOD/FMOD/fsbank/lib/x64/fsbank.dll"
Libraries["FMOD_WIN64_FSBANK"] = "3rdparty/FMOD/FMOD/fsbank/lib/x64/fsbank_vc.lib"
Libraries["FMOD_WIN64_LIBFSVORBIS_DLL"] = "3rdparty/FMOD/FMOD/fsbank/lib/x64/libfsvorbis64.dll"
Libraries["FMOD_WIN64_OPUS_DLL"] = "3rdparty/FMOD/FMOD/fsbank/lib/x64/opus.dll"

Libraries["FMOD_WIN64_STUDIO_DLL_D"] = "3rdparty/FMOD/FMOD/studio/lib/x64/fmodstudioL.dll"
Libraries["FMOD_WIN64_STUDIO_D"] = "3rdparty/FMOD/FMOD/studio/lib/x64/fmodstudioL_vc.lib"

Libraries["FMOD_WIN64_STUDIO_DLL_D"] = "3rdparty/FMOD/FMOD/studio/lib/x64/fmodstudio.dll"
Libraries["FMOD_WIN64_STUDIO_D"] = "3rdparty/FMOD/FMOD/studio/lib/x64/fmodstudio_vc.lib"