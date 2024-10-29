#ifndef SOGE_CORE_SINGLEDELEGATE_HPP
#define SOGE_CORE_SINGLEDELEGATE_HPP

#include "SOGE/Core/Delegate.hpp"

#include <functional>
#include <optional>


namespace soge
{
    template <typename Prototype>
    class SingleDelegate;

    template <typename R, typename... Args>
    class SingleDelegate<R(Args...)> : public Delegate<R(Args...)>
    {
    private:
        using Parent = Delegate<R(Args...)>;
        using Prototype = typename Parent::Prototype;
        using ReturnType = typename Parent::ReturnType;

        using CallbackInternal = std::function<Prototype>;
        CallbackInternal m_callback;

    public:
        using Callback = CallbackInternal;

        explicit SingleDelegate() noexcept = default;
        ~SingleDelegate() noexcept override = default;

        explicit SingleDelegate(const SingleDelegate&) = default;
        SingleDelegate& operator=(const SingleDelegate&) = default;

        explicit SingleDelegate(SingleDelegate&&) noexcept = default;
        SingleDelegate& operator=(SingleDelegate&&) noexcept = default;

        template <typename F>
        requires std::is_invocable_r_v<R, F, Args...>
        SingleDelegate(F&& aFunction);

        template <typename F>
        requires std::is_invocable_r_v<R, F, Args...>
        SingleDelegate& operator=(F&& aFunction);

        [[nodiscard]]
        bool IsEmpty() const override;

        Callback Bind(Callback&& aCallback);
        Callback Clear();

        [[nodiscard]]
        std::optional<ReturnType> InvokeIfBound(Args&&... args) const
        requires not std::same_as<ReturnType, void>;

        [[nodiscard]]
        bool InvokeIfBound(Args&&... args) const
        requires std::same_as<ReturnType, void>;

        ReturnType operator()(Args&&... args) const override;
    };

    template <typename R, typename... Args>
    template <typename F>
    requires std::is_invocable_r_v<R, F, Args...>
    SingleDelegate<R(Args...)>::SingleDelegate(F&& aFunction) : m_callback{std::forward<F>(aFunction)}
    {
    }

    template <typename R, typename... Args>
    template <typename F>
    requires std::is_invocable_r_v<R, F, Args...>
    auto SingleDelegate<R(Args...)>::operator=(F&& aFunction) -> SingleDelegate&
    {
        m_callback = std::forward<F>(aFunction);
        return *this;
    }

    template <typename R, typename... Args>
    bool SingleDelegate<R(Args...)>::IsEmpty() const
    {
        return m_callback == nullptr;
    }

    template <typename R, typename... Args>
    auto SingleDelegate<R(Args...)>::Bind(Callback&& aCallback) -> Callback
    {
        Callback old = std::move(m_callback);
        m_callback = std::move(aCallback);
        return old;
    }

    template <typename R, typename... Args>
    auto SingleDelegate<R(Args...)>::Clear() -> Callback
    {
        return Bind(Callback{});
    }

    template <typename R, typename... Args>
    auto SingleDelegate<R(Args...)>::InvokeIfBound(Args&&... args) const -> std::optional<ReturnType>
    requires not std::same_as<ReturnType, void>
    {
        if (m_callback == nullptr)
        {
            return std::nullopt;
        }
        return m_callback.operator()(std::forward<Args>(args)...);
    }

    template <typename R, typename... Args>
    bool SingleDelegate<R(Args...)>::InvokeIfBound(Args&&... args) const
    requires std::same_as<ReturnType, void>
    {
        if (m_callback == nullptr)
        {
            return false;
        }

        m_callback.operator()(std::forward<Args>(args)...);
        return true;
    }

    template <typename R, typename... Args>
    auto SingleDelegate<R(Args...)>::operator()(Args&&... args) const -> ReturnType
    {
        return m_callback.operator()(std::forward<Args>(args)...);
    }
}

#endif // SOGE_CORE_SINGLEDELEGATE_HPP
