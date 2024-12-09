#ifndef SOGE_SOUND_IMPL_FMOD_FMODEXCEPTION_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODEXCEPTION_HPP

#include <exception>
#include <FMOD/fmod.hpp>


namespace soge
{
    class FMODException : public std::exception
    {
    private:
        FMOD_RESULT m_fmodResult;

    public:
        FMODException(FMOD_RESULT aFmodResult);
        ~FMODException() = default;
        const char* what() const noexcept override;

    };

    inline void FMODThrowIfFailed(FMOD_RESULT aFmodResult)
    {
        if (aFmodResult != FMOD_OK)
        {
            auto except = FMODException(aFmodResult);
            SOGE_ERROR_LOG("FMOD Exception: {0}", except.what());
            throw except;
        }
    }
}

#endif // !SOGE_SOUND_IMPL_FMOD_FMODEXCEPTION_HPP
