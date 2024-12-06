#ifndef SOGE_DI_DEPENDENCY_HPP
#define SOGE_DI_DEPENDENCY_HPP

#include <kangaru/autowire.hpp>
#include <kangaru/operator.hpp>
#include <kangaru/service.hpp>


namespace soge::di
{
    template <typename T>
    using DependencyDefinition = kgr::mapped_service_t<T>;

    template <typename T>
    concept Dependency = requires { typename DependencyDefinition<T>; };

    namespace detail
    {
        template <typename... Deps>
        using Dependencies =
            std::conditional_t<sizeof...(Deps) == 0, kgr::autowire, kgr::dependency<kgr::mapped_service_t<Deps>...>>;
    }

    namespace definition
    {
        template <typename T, typename... Deps>
        using Factory = kgr::service<T, detail::Dependencies<Deps...>>;

        template <typename T, typename... Deps>
        using Single = kgr::single_service<T, detail::Dependencies<Deps...>>;

        template <typename T>
        using External = kgr::extern_service<T>;

        template <typename T>
        requires std::is_abstract_v<T>
        using Abstract = kgr::abstract_service<T>;
    }

    namespace df = definition;

    namespace tag
    {
        template <typename T>
        using Supplied = kgr::supplied;

        template <typename T>
        requires std::is_polymorphic_v<T>
        using Polymorphic = kgr::polymorphic;

        template <typename T>
        using Final = kgr::final;

        template <typename T, typename... Parents>
        requires(std::derived_from<T, Parents> && ...)
        using Overrides = kgr::overrides<kgr::mapped_service_t<Parents>...>;

        // template <typename T, typename Child>
        // requires std::derived_from<Child, T>
        // using DefaultsTo = kgr::defaults_to<kgr::mapped_service_t<Child>>;
    }

    template <typename T>
    concept PolymorphicDependency = Dependency<T> && std::derived_from<DependencyDefinition<T>, kgr::polymorphic>;

    template <typename T>
    concept AbstractDependency = Dependency<T> && std::derived_from<DependencyDefinition<T>, kgr::abstract>;

    template <Dependency T>
    using Lazy = kgr::lazy<DependencyDefinition<T>>;

    template <PolymorphicDependency T>
    using PolymorphicRange = kgr::override_range<kgr::detail::override_iterator<DependencyDefinition<T>>>;
}

// Registers a class of given namespace as a dependency with the given definition globally
// Should be called in global namespace
#define SOGE_DI_REGISTER_NS(ns, T, ...)                                                                                \
    namespace soge::di::detail                                                                                         \
    {                                                                                                                  \
        struct Definition_##ns##T : __VA_ARGS__                                                                        \
        {                                                                                                              \
        };                                                                                                             \
    }                                                                                                                  \
    template <>                                                                                                        \
    struct kgr::detail::map_entry<kgr::map<>, ns::T, void>                                                             \
    {                                                                                                                  \
        using mapped_service = soge::di::detail::Definition_##ns##T;                                                   \
    };

// Registers a class as a dependency with the given definition globally
// Should be called in global namespace
#define SOGE_DI_REGISTER(T, ...) SOGE_DI_REGISTER_NS(, T, __VA_ARGS__)

#endif // SOGE_DI_DEPENDENCY_HPP
