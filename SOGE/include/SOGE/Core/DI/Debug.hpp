#ifndef SOGE_CORE_DI_DEBUG_HPP
#define SOGE_CORE_DI_DEBUG_HPP

#include "SOGE/Core/DI/Dependency.hpp"

#include <kangaru/debug.hpp>


namespace soge::di::debug
{
    template <Dependency T, typename Arg, typename... Args>
    void Provide(Arg&& arg, Args&&...)
    {
        using Service = DependencyDefinition<T>;

        // kgr::debug::service<Service, Arg, Args...>(std::forward<Arg>(arg));
        if constexpr (std::is_constructible_v<kgr::detail::service_error<Service, Args...>, Arg>)
        {
            (void)kgr::detail::service_error<Service, Args...>{std::forward<Arg>(arg)};
        }
    }

    template <Dependency T>
    void Provide()
    {
        using Service = DependencyDefinition<T>;

        // kgr::debug::service<Service>();
        if constexpr (std::is_default_constructible_v<kgr::detail::service_error<Service>>)
        {
            (void)kgr::detail::service_error<Service>{};
        }
    }
}

#endif // SOGE_CORE_DI_DEBUG_HPP
