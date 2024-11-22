#include "sogepch.hpp"
#include "SOGE/Event/InputEvents.hpp"


namespace soge
{
    KeyboardEventBase::KeyboardEventBase(Key aKeyObj) noexcept : m_keyObj(aKeyObj)
    {
    }

    Key KeyboardEventBase::GetKey() const
    {
        return m_keyObj;
    }

    KeyPressedEvent::KeyPressedEvent(Key aPressedKey, int aRepeatCount) noexcept
        : KeyboardEventBase(aPressedKey), m_repeatCount(aRepeatCount)
    {
    }

    int KeyPressedEvent::GetRepeatCount() const
    {
        return m_repeatCount;
    }

    KeyReleasedEvent::KeyReleasedEvent(Key aReleasedKey) noexcept : KeyboardEventBase(aReleasedKey)
    {
    }

}
