#ifndef SOGE_SOUND_IMPL_FMOD_FMODSOUND_HPP
#define SOGE_SOUND_IMPL_FMOD_FMODSOUND_HPP

#include "SOGE/Sound/SoundResource.hpp"
#include <FMOD/fmod.hpp>


namespace soge
{
    class FMODConfig;

    /**
    * @class FMODSound FMODSound.hpp "SOGE/Sound/Impl/FMOD/FMODSound.hpp"
    * @brief Holds FMOD sound data and provides methods to operate with it.
    */
    class FMODSound
    {
    private:
        FMOD::Sound* m_fmodSound;
        SoundResource& m_soundResource;

    public:
        /**
        * @public
        * @brief Initialize class with reference to it's sound resource.
        * @param[in] aSoundResource - Reference to sound resource located in filesystem.
        */
        FMODSound(SoundResource& aSoundResource);

        /**
        * @public
        * @brief Initialize class with pointer to existed FMOD::Sound and it's linked sound resource.
        * @param[in] aFMODSound - Pointer to native FMOD::Sound.
        * @param[in] aSoundResource - Reference to sound resource located in filesystem.
        */
        FMODSound(FMOD::Sound* aFMODSound, SoundResource& aSoundResource);
        ~FMODSound();

        /**
        * @public
        * @brief Loads sound resource to FMOD system.
        * @param[in] aFMODSystem - Pointer to FMOD::System.
        */
        bool Load(FMOD::System* aFMODSystem, const FMODConfig& aFMODConfig);

        /**
        * @public
        * @brief Reloads sound with new sound resource data.
        * @param[in] aFMODSystem - Pointer to FMOD::System.
        * @note Not implemented yet.
        */
        bool Reload(FMOD::System* aFMODSystem, const FMODConfig& aFMODConfig);

        /**
        * @public
        * @brief Clear sound data, but not destroy it. Will wait to load again.
        * @param[in] aFMODSystem - Pointer to FMOD::System.
        * @note Not implemented yet.
        */
        bool Unload(FMOD::System* aFMODSystem);

        /**
        * @public
        * @brief Gets native FMOD::Sound.
        * @returns Native FMOD::Sound structure.
        */
        FMOD::Sound* GetSound() const;

    };
}

#endif // !SOGE_SOUND_IMPL_FMOD_FMODSOUND_HPP
