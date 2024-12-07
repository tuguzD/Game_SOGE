#ifndef SOGE_DELEGATE_DELEGATE_HPP
#define SOGE_DELEGATE_DELEGATE_HPP


namespace soge
{
    template <typename Prototype>
    class Delegate;

    template <typename R, typename... Args>
    class Delegate<R(Args...)>
    {
    public:
        using Prototype = R(Args...);

        using ReturnType = R;
        using Arguments = std::tuple<Args...>;

        constexpr virtual ~Delegate() = default;

        [[nodiscard]]
        constexpr virtual bool IsEmpty() const;

        [[nodiscard]]
        constexpr virtual ReturnType
        operator()(Args&&... args) const = 0;

        constexpr operator bool() const;
    };

    template <typename R, typename... Args>
    constexpr bool Delegate<R(Args...)>::IsEmpty() const
    {
        return true;
    }

    template <typename R, typename... Args>
    constexpr Delegate<R(Args...)>::operator bool() const
    {
        return !IsEmpty();
    }
}

#endif // SOGE_DELEGATE_DELEGATE_HPP
