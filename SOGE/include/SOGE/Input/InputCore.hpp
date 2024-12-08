#ifndef SOGE_INPUT_INPUTCORE_HPP
#define SOGE_INPUT_INPUTCORE_HPP

#include "SOGE/Input/Device/Gamepad.hpp"
#include "SOGE/Input/Device/Keyboard.hpp"
#include "SOGE/Input/Device/Mouse.hpp"
#include "SOGE/Input/KeyMapManager.hpp"


namespace soge
{
    class EventModule;

    class InputCore
    {
    private:
        EventModule* m_eventModule;

    protected:
        [[nodiscard]]
        constexpr EventModule* GetEventModule() const noexcept;

        UniquePtr<Mouse> m_mouse;
        UniquePtr<Gamepad> m_gamepad;
        UniquePtr<Keyboard> m_keyboard;
        UniquePtr<KeyMapManager> m_keyMapManager;

    public:
        constexpr explicit InputCore(EventModule* aEventModule) noexcept;
        constexpr virtual ~InputCore() noexcept = default;

        constexpr explicit InputCore(const InputCore&) noexcept = delete;
        constexpr InputCore& operator=(const InputCore&) noexcept = delete;

        constexpr explicit InputCore(InputCore&&) noexcept = default;
        constexpr InputCore& operator=(InputCore&&) noexcept = default;

        virtual void LockInput(bool aLockInput) = 0;
        virtual void UseRelativeMouseMode(bool aLockMouse) = 0;
        virtual bool IsAnyButtonPressed() = 0;

        virtual void BeginUpdateInput() = 0;
        virtual void EndUpdateInput() = 0;
        virtual void SetPauseUpdate(bool aIsPauseNeeded) = 0;

        virtual bool IsKeyPressed(const Key& aKey) const = 0;
        virtual bool IsKeyReleased(const Key& aKey) const = 0;

        virtual Keyboard* GetKeyboard() const   = 0;
        virtual Gamepad* GetGamepad() const     = 0;
        virtual Mouse* GetMouse() const         = 0;

    };

    constexpr InputCore::InputCore(EventModule* aEventModule) noexcept : m_eventModule(aEventModule)
    {
    }

    constexpr EventModule* InputCore::GetEventModule() const noexcept
    {
        return m_eventModule;
    }
}

#endif // SOGE_INPUT_INPUTCORE_HPP
