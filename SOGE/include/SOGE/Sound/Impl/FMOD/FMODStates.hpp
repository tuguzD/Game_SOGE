#ifndef SOGE_SOUND_IMPL_FMOD_FMODSTATES_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODSTATES_HPP


namespace soge
{
    enum FMODState
    {
        FMODState_SuccessLoad,
        FMODState_AlreadyLoaded,
        FMODState_NotLoaded,
        FMODState_Playing,
        FMODState_Paused
    };
}

#endif // !SOGE_SOUND_IMPL_FMOD_FMODSTATES_HPP
