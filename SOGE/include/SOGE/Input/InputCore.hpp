#ifndef SOGE_INPUT_INPUTCORE_HPP
#define SOGE_INPUT_INPUTCORE_HPP

#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Input/Device/Gamepad.hpp"
#include "SOGE/Input/Device/Keyboard.hpp"
#include "SOGE/Input/Device/Mouse.hpp"
#include "SOGE/Input/KeyMapManager.hpp"


namespace soge
{
    class InputCore
    {
    private:
        eastl::reference_wrapper<EventModule> m_eventModule;

    protected:
        [[nodiscard]]
        EventModule& GetEventModule() const noexcept;

        UniquePtr<Mouse> m_mouse;
        UniquePtr<Gamepad> m_gamepad;
        UniquePtr<Keyboard> m_keyboard;
        UniquePtr<KeyMapManager> m_keyMapManager;

    public:
        explicit InputCore(EventModule& aEventModule) noexcept;

        constexpr explicit InputCore(const InputCore&) noexcept = delete;
        constexpr InputCore& operator=(const InputCore&) noexcept = delete;

        constexpr explicit InputCore(InputCore&&) noexcept = default;
        constexpr InputCore& operator=(InputCore&&) noexcept = default;

        constexpr virtual ~InputCore() noexcept = default;

        virtual void LockInput(bool aLockInput) = 0;
        virtual void UseRelativeMouseMode(bool aLockMouse) = 0;
        virtual bool IsAnyButtonPressed() = 0;

        virtual void BeginUpdateInput() = 0;
        virtual void EndUpdateInput() = 0;
        virtual void SetPauseUpdate(bool aIsPauseNeeded) = 0;

        [[nodiscard]]
        virtual bool IsKeyPressed(const Key& aKey) const = 0;
        [[nodiscard]]
        virtual bool IsKeyReleased(const Key& aKey) const = 0;

        [[nodiscard]]
        virtual Keyboard* GetKeyboard() const = 0;
        [[nodiscard]]
        virtual Gamepad* GetGamepad() const = 0;
        [[nodiscard]]
        virtual Mouse* GetMouse() const = 0;
    };

    inline InputCore::InputCore(EventModule& aEventModule) noexcept : m_eventModule(aEventModule)
    {
    }

    inline EventModule& InputCore::GetEventModule() const noexcept
    {
        return m_eventModule.get();
    }
}

SOGE_DI_REGISTER_NS(soge, InputCore, df::Abstract<InputCore>)

#endif // SOGE_INPUT_INPUTCORE_HPP
