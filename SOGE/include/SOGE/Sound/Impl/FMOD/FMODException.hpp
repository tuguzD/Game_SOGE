#ifndef SOGE_SOUND_IMPL_FMOD_FMODEXCEPTION_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODEXCEPTION_HPP

#include <exception>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>


namespace soge
{
    /**
    * @class FMODException FMODException.hpp "SOGE/Sound/Impl/FMOD/FMODException.hpp"
    * @extends std::exception
    * @brief General exception class for FMOD_RESULT errors.
    */
    class FMODException : public std::exception
    {
    private:
        FMOD_RESULT m_fmodResult;

    public:
        /**
        * @public
        * @brief Constructs FMODException from FMOD_RESULT.
        * @param[in] aFmodResult - Native FMOD_RESULT structure.
        */
        FMODException(FMOD_RESULT aFmodResult);
        ~FMODException() = default;

        /**
        * @public
        * @brief Gets FMOD_RESULT error description if FMODException was thrown.
        * @returns String with FMOD_RESULT error description.
        */
        const char* what() const noexcept override;

    };

    /**
    * @public
    * @brief Global function to check FMOD_RESULT for errors and throw FMODException if result was bad\n.
    *
    * Example:
    * @code
    *   FMODThrowIfFailed(FMOD::System::create()); // Will throw exception if FMOD_RESULT was not FMOD_OK
    * @endcode
    */
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
