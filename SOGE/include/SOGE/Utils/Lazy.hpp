#ifndef SOGE_UTILS_LAZY_HPP
#define SOGE_UTILS_LAZY_HPP

#include <concepts>


namespace soge
{
    // Utility for lazy initialization of a value from given lambda
    // https://jguegant.github.io/blogs/tech/performing-try-emplace.html
    template <std::invocable F>
    class LazyConvertInvoke
    {
    private:
        F m_factory;

    public:
        using ResultType = std::invoke_result_t<F&>;
        using ConstResultType = std::invoke_result_t<const F&>;

        explicit constexpr LazyConvertInvoke(F&& aFactory) : m_factory(std::move(aFactory))
        {
        }

        constexpr operator ResultType() noexcept(std::is_nothrow_invocable_v<F&>)
        {
            return m_factory();
        }

        constexpr operator ConstResultType() const noexcept(std::is_nothrow_invocable_v<const F&>)
        {
            return m_factory();
        }
    };
}

#endif // SOGE_UTILS_LAZY_HPP
