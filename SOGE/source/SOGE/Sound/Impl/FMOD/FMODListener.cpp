#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODListener.hpp"


namespace soge
{
    FMODListener::FMODListener()
    {
        m_listenerHeadPos       = { 0.0f, 0.0f, -1.0f };
        m_listenerForwardVector = { 0.0f, 0.0f, 1.0f };
        m_listenerUpwardVector  = { 0.0f, 1.0f, 0.0f };
    }

    void FMODListener::Set3DListenerPosition(const FMOD_VECTOR& aPos,
                                             const FMOD_VECTOR& aForwardVec,
                                             const FMOD_VECTOR& aUpwardVec)
    {
        m_listenerHeadPos = aPos;
        m_listenerForwardVector = aForwardVec;
        m_listenerUpwardVector = aUpwardVec;
    }

    FMOD_VECTOR FMODListener::GetListenerPosition() const
    {
        return m_listenerHeadPos;
    }

    FMOD_VECTOR FMODListener::GetListenerForwardVector() const
    {
        return m_listenerForwardVector;
    }

    FMOD_VECTOR FMODListener::GetListenerUpwardVector() const
    {
        return m_listenerUpwardVector;
    }
}
