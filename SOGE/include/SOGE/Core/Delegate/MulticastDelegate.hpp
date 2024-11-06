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
        using Prototype = R(Args...);
        using ReturnType = R;

        using FunctionList = eventpp::CallbackList<R(Args...)>;
        FunctionList m_functions;

    public:
        using Function = typename FunctionList::Callback;
        using FunctionHandle = typename FunctionList::Handle;

        explicit MulticastDelegate() noexcept = default;
        ~MulticastDelegate() override = default;

        explicit MulticastDelegate(const MulticastDelegate&) = default;
        MulticastDelegate& operator=(const MulticastDelegate&) = default;

        explicit MulticastDelegate(MulticastDelegate&&) noexcept = default;
        MulticastDelegate& operator=(MulticastDelegate&&) noexcept = default;

        [[nodiscard]]
        bool IsEmpty() const override;

        template <typename F>
        requires std::is_invocable_r_v<R, F, Args...>
        FunctionHandle PushBack(F&& aFunction);

        template <typename F>
        requires std::is_invocable_r_v<R, F, Args...>
        FunctionHandle PushFront(F&& aFunction);

        template <typename F>
        requires std::is_invocable_r_v<R, F, Args...>
        FunctionHandle Insert(F&& aFunction, const FunctionHandle& aBefore);

        bool Remove(const FunctionHandle& aHandle);

        [[nodiscard]]
        bool Contains(const FunctionHandle& aHandle) const;

        void Clear();

        void operator()(Args&&... args) const override;

        template <typename F>
        requires std::is_invocable_r_v<R, F, Args...>
        MulticastDelegate& operator+=(F&& aFunction);

        MulticastDelegate& operator-=(const FunctionHandle& aHandle);
    };

    template <typename R, typename... Args>
    bool MulticastDelegate<R(Args...)>::IsEmpty() const
    {
        return m_functions.empty();
    }

    template <typename R, typename... Args>
    template <typename F>
    requires std::is_invocable_r_v<R, F, Args...>
    auto MulticastDelegate<R(Args...)>::PushBack(F&& aFunction) -> FunctionHandle
    {
        return m_functions.append(std::forward<F>(aFunction));
    }

    template <typename R, typename... Args>
    template <typename F>
    requires std::is_invocable_r_v<R, F, Args...>
    auto MulticastDelegate<R(Args...)>::PushFront(F&& aFunction) -> FunctionHandle
    {
        return m_functions.prepend(std::forward<F>(aFunction));
    }

    template <typename R, typename... Args>
    template <typename F>
    requires std::is_invocable_r_v<R, F, Args...>
    auto MulticastDelegate<R(Args...)>::Insert(F&& aFunction, const FunctionHandle& aBefore) -> FunctionHandle
    {
        if (!m_functions.ownsHandle(aBefore))
        {
            return m_functions.append(std::forward<F>(aFunction));
        }
        return m_functions.insert(std::forward<F>(aFunction), aBefore);
    }

    template <typename R, typename... Args>
    bool MulticastDelegate<R(Args...)>::Remove(const FunctionHandle& aHandle)
    {
        if (!m_functions.ownsHandle(aHandle))
        {
            return false;
        }
        return m_functions.remove(aHandle);
    }

    template <typename R, typename... Args>
    bool MulticastDelegate<R(Args...)>::Contains(const FunctionHandle& aHandle) const
    {
        return m_functions.ownsHandle(aHandle);
    }

    template <typename R, typename... Args>
    void MulticastDelegate<R(Args...)>::Clear()
    {
        FunctionList functions;
        m_functions.swap(functions);
    }

    template <typename R, typename... Args>
    void MulticastDelegate<R(Args...)>::operator()(Args&&... args) const
    {
        m_functions.operator()(std::forward<Args>(args)...);
    }

    template <typename R, typename... Args>
    template <typename F>
    requires std::is_invocable_r_v<R, F, Args...>
    auto MulticastDelegate<R(Args...)>::operator+=(F&& aFunction) -> MulticastDelegate&
    {
        PushBack(std::forward<F>(aFunction));
        return *this;
    }

    template <typename R, typename... Args>
    auto MulticastDelegate<R(Args...)>::operator-=(const FunctionHandle& aHandle) -> MulticastDelegate&
    {
        Remove(aHandle);
        return *this;
    }
}

#endif // SOGE_CORE_DELEGATE_MULTICASTDELEGATE_HPP
