#ifndef SOGE_CORE_DELEGATE_MULTICASTDELEGATE_HPP
#define SOGE_CORE_DELEGATE_MULTICASTDELEGATE_HPP

#include "SOGE/Core/Delegate/Delegate.hpp"

#include <eventpp/callbacklist.h>


namespace soge
{
    template <typename Prototype>
    class MulticastDelegate;

    template <typename R, typename... Args>
    class MulticastDelegate<R(Args...)> : public Delegate<void(Args...)>
    {
    private:
        using Parent = Delegate<void(Args...)>;
        using Prototype = typename Parent::Prototype;
        using ReturnType = typename Parent::ReturnType;

        using Callbacks = eventpp::CallbackList<R(Args...)>;
        Callbacks m_callbacks;

    public:
        using Callback = typename Callbacks::Callback;
        using CallbackHandle = typename Callbacks::Handle;

        explicit MulticastDelegate() noexcept = default;
        ~MulticastDelegate() noexcept override = default;

        explicit MulticastDelegate(const MulticastDelegate&) = default;
        MulticastDelegate& operator=(const MulticastDelegate&) = default;

        explicit MulticastDelegate(MulticastDelegate&&) noexcept = default;
        MulticastDelegate& operator=(MulticastDelegate&&) noexcept = default;

        [[nodiscard]]
        bool IsEmpty() const override;

        CallbackHandle Append(Callback&& aCallback);
        CallbackHandle Prepend(Callback&& aCallback);
        CallbackHandle Insert(Callback&& aCallback, const CallbackHandle& aHandleBefore);
        bool Remove(const CallbackHandle& aHandle);

        [[nodiscard]]
        bool Contains(const CallbackHandle& aHandle) const;

        void operator()(Args&&... args) const override;
    };

    template <typename R, typename... Args>
    bool MulticastDelegate<R(Args...)>::IsEmpty() const
    {
        return m_callbacks.empty();
    }

    template <typename R, typename... Args>
    auto MulticastDelegate<R(Args...)>::Append(Callback&& aCallback) -> CallbackHandle
    {
        return m_callbacks.append(std::move(aCallback));
    }

    template <typename R, typename... Args>
    auto MulticastDelegate<R(Args...)>::Prepend(Callback&& aCallback) -> CallbackHandle
    {
        return m_callbacks.prepend(std::move(aCallback));
    }

    template <typename R, typename... Args>
    auto MulticastDelegate<R(Args...)>::Insert(Callback&& aCallback, const CallbackHandle& aHandleBefore)
        -> CallbackHandle
    {
        if (!m_callbacks.ownsHandle(aHandleBefore))
        {
            return m_callbacks.append(std::move(aCallback));
        }
        return m_callbacks.insert(std::move(aCallback), aHandleBefore);
    }

    template <typename R, typename... Args>
    bool MulticastDelegate<R(Args...)>::Remove(const CallbackHandle& aHandle)
    {
        if (!m_callbacks.ownsHandle(aHandle))
        {
            return false;
        }
        return m_callbacks.remove(aHandle);
    }

    template <typename R, typename... Args>
    bool MulticastDelegate<R(Args...)>::Contains(const CallbackHandle& aHandle) const
    {
        return m_callbacks.ownsHandle(aHandle);
    }

    template <typename R, typename... Args>
    void MulticastDelegate<R(Args...)>::operator()(Args&&... args) const
    {
        m_callbacks.operator()(std::forward<Args>(args)...);
    }
}

#endif // SOGE_CORE_DELEGATE_MULTICASTDELEGATE_HPP
