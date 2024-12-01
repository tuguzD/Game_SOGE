#include "sogepch.hpp"
#include "SOGE/Event/InputEvents.hpp"


namespace soge
{
    KeyEventBase::KeyEventBase(const Key aKeyObj) noexcept : m_keyObj(aKeyObj)
    {
    }

    Key KeyEventBase::GetKey() const
    {
        return m_keyObj;
    }

    //////////////////////
    // Keyboard Event
    /////////////////////

    KeyPressedEvent::KeyPressedEvent(const Key aPressedKey, int aRepeatCount) noexcept
        : KeyEventBase(aPressedKey), m_repeatCount(aRepeatCount)
    {
    }

    int KeyPressedEvent::GetRepeatCount() const
    {
        return m_repeatCount;
    }

    KeyReleasedEvent::KeyReleasedEvent(const Key aReleasedKey) noexcept : KeyEventBase(aReleasedKey)
    {
    }

    //////////////////////
    // Mouse Event
    /////////////////////

    MouseButtonPressedEvent::MouseButtonPressedEvent(const Key aPressedButton, int aRepeatCount) noexcept
        : KeyEventBase(aPressedButton), m_repeatCount(aRepeatCount)
    {
    }

    int MouseButtonPressedEvent::GetRepeatCount() const
    {
        return m_repeatCount;
    }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(const Key aReleasedButton) noexcept : KeyEventBase(aReleasedButton)
    {
    }

    MouseMovedEvent::MouseMovedEvent(const FloatPair aCoords) noexcept : m_coords(aCoords)
    {
    }

    MouseMovedEvent::MouseMovedEvent(float aX, float aY) noexcept : m_coords(aX, aY)
    {
    }

    MouseMovedEvent::FloatPair MouseMovedEvent::GetCoords() const
    {
        return m_coords;
    }

    MouseWheelEvent::MouseWheelEvent(float aOffset) noexcept : m_offset(aOffset)
    {
    }

    float MouseWheelEvent::GetOffset() const
    {
        return m_offset;
    }

}
