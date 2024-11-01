#ifndef SOGE_CORE_DELEGATE_SINGLEDELEGATE_HPP
#define SOGE_CORE_DELEGATE_SINGLEDELEGATE_HPP

#include "SOGE/Core/Delegate/Delegate.hpp"

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

        using FunctionInternal = std::function<Prototype>;
        FunctionInternal m_function;

    public:
        using Function = FunctionInternal;

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

        template <typename F>
        requires std::is_invocable_r_v<R, F, Args...>
        Function Bind(F&& aFunction);

        Function Clear();

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
    SingleDelegate<R(Args...)>::SingleDelegate(F&& aFunction) : m_function{std::forward<F>(aFunction)}
    {
    }

    template <typename R, typename... Args>
    template <typename F>
    requires std::is_invocable_r_v<R, F, Args...>
    auto SingleDelegate<R(Args...)>::operator=(F&& aFunction) -> SingleDelegate&
    {
        m_function = std::forward<F>(aFunction);
        return *this;
    }

    template <typename R, typename... Args>
    bool SingleDelegate<R(Args...)>::IsEmpty() const
    {
        return m_function == nullptr;
    }

    template <typename R, typename... Args>
    template <typename F>
    requires std::is_invocable_r_v<R, F, Args...>
    auto SingleDelegate<R(Args...)>::Bind(F&& aFunction) -> Function
    {
        Function function{std::forward<F>(aFunction)};
        std::swap(m_function, function);
        return function;
    }

    template <typename R, typename... Args>
    auto SingleDelegate<R(Args...)>::Clear() -> Function
    {
        return Bind(Function{});
    }

    template <typename R, typename... Args>
    auto SingleDelegate<R(Args...)>::InvokeIfBound(Args&&... args) const -> std::optional<ReturnType>
    requires not std::same_as<ReturnType, void>
    {
        if (m_function == nullptr)
        {
            return std::nullopt;
        }
        return m_function.operator()(std::forward<Args>(args)...);
    }

    template <typename R, typename... Args>
    bool SingleDelegate<R(Args...)>::InvokeIfBound(Args&&... args) const
    requires std::same_as<ReturnType, void>
    {
        if (m_function == nullptr)
        {
            return false;
        }

        m_function.operator()(std::forward<Args>(args)...);
        return true;
    }

    template <typename R, typename... Args>
    auto SingleDelegate<R(Args...)>::operator()(Args&&... args) const -> ReturnType
    {
        return m_function.operator()(std::forward<Args>(args)...);
    }
}

#endif // SOGE_CORE_DELEGATE_SINGLEDELEGATE_HPP
