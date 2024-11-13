#ifndef SOGE_CORE_DI_DEPENDENCY_HPP
#define SOGE_CORE_DI_DEPENDENCY_HPP

#include <kangaru/autowire.hpp>
#include <kangaru/debug.hpp>
#include <kangaru/operator.hpp>
#include <kangaru/service.hpp>


namespace soge::di
{
    template <typename T>
    using DependencyDefinition = kgr::mapped_service_t<T>;

    template <typename T>
    concept Dependency = requires { typename DependencyDefinition<T>; };

    template <Dependency T>
    void Debug()
    {
        using Service = DependencyDefinition<T>;

        kgr::debug::service<Service>();
    }

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

    template <typename T>
    concept PolymorphicDependency = Dependency<T> && std::derived_from<DependencyDefinition<T>, kgr::polymorphic>;

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

    template <Dependency T>
    using Lazy = kgr::lazy<DependencyDefinition<T>>;
}

// Registers a class as a dependency with the given definition globally
// Should be called in global namespace
#define SOGE_DI_REGISTER(T, ...)                                                                                       \
    namespace soge::di::detail                                                                                         \
    {                                                                                                                  \
        struct Definition##T;                                                                                          \
    }                                                                                                                  \
    auto service_map(const T&) -> soge::di::detail::Definition##T;                                                     \
    namespace soge::di::detail                                                                                         \
    {                                                                                                                  \
        struct Definition##T : __VA_ARGS__                                                                             \
        {                                                                                                              \
        };                                                                                                             \
    }                                                                                                                  \
    template <>                                                                                                        \
    requires std::derived_from<soge::di::detail::Definition##T, soge::di::df::External<T>>                             \
    struct kgr::detail::map_entry<kgr::map<>, T, void>                                                                 \
    {                                                                                                                  \
        using mapped_service = soge::di::detail::Definition##T;                                                        \
    };

#endif // SOGE_CORE_DI_DEPENDENCY_HPP
