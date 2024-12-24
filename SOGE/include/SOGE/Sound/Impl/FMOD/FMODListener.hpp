#ifndef SOGE_SOUND_IMPL_FMOD_FMOD_LISTENER_HPP
#define SOGE_SOUND_IMPL_FMOD_FMOD_LISTENER_HPP

#include <FMOD/fmod.hpp>


namespace soge
{
    /**
    * @class FMODListener FMODListener.hpp "SOGE/Sound/Impl/FMOD/FMODListener.hpp"
    * @brief Holds 3D listener positioning data in a 3D world space.
    */
    class FMODListener
    {
    private:
        FMOD_VECTOR m_listenerHeadPos;
        FMOD_VECTOR m_listenerForwardVector;
        FMOD_VECTOR m_listenerUpwardVector;

    public:
        FMODListener();
        ~FMODListener() = default;

        /**
        * @public
        * @brief Sets the position of the listener in 3D scene.
        * @param[in] aPos - 3D translation of the listener.
        * @param[in] aForwardVec - Forward angle character is looking in.
        * @param[in] aUpwardVec - Up which must be perpendicular to forward vector.
        */
        void Set3DListenerPosition(const FMOD_VECTOR& aPos,
                                   const FMOD_VECTOR& aForwardVec,
                                   const FMOD_VECTOR& aUpwardVec);

        /**
        * @public
        * @brief Gets 3d translation of listener.
        * @returns 3D vector with current listener translation.
        */
        FMOD_VECTOR GetListenerPosition() const;

        /**
         * @public
         * @brief Gets 3d listener forward angle listener looking in.
         * @returns 3D vector with current listener forward vector value.
         */
        FMOD_VECTOR GetListenerForwardVector() const;

        /**
         * @public
         * @brief Gets 3d listener upward vector.
         * @returns 3D vector with listener upward vector value.
         */
        FMOD_VECTOR GetListenerUpwardVector() const;

    };
}

#endif // !SOGE_SOUND_IMPL_FMOD_FMOD_LISTENER_HPP
