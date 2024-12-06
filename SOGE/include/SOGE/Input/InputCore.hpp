#ifndef SOGE_INPUT_INPUTCORE_HPP
#define SOGE_INPUT_INPUTCORE_HPP


namespace soge
{
    class Keyboard;
    class Gamepad;
    class Mouse;

    class InputCore
    {
    public:
        constexpr explicit InputCore() noexcept = default;
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

        virtual Mouse* CreateMouse() = 0;
        virtual Gamepad* CreateGamepad() = 0;
        virtual Keyboard* CreateKeyboard() = 0;
    };
}

#endif // SOGE_INPUT_INPUTCORE_HPP
