#ifndef SOGE_CORE_MODULE_HPP
#define SOGE_CORE_MODULE_HPP

#include "SOGE/Core/DI/Container.hpp"


namespace soge
{
    class Module;

    template <typename T>
    concept DerivedFromModule = std::derived_from<T, Module>;

    class Module
    {
    public:
        constexpr explicit Module() = default;
        constexpr virtual ~Module() = default;

        explicit Module(const Module&) = delete;
        Module& operator=(const Module&) = delete;

        explicit Module(Module&&) = default;
        Module& operator=(Module&&) = default;

        constexpr virtual void Load(di::Container& aContainer) = 0;
        constexpr virtual void UnLoad(di::Container& aContainer) = 0;
    };

    namespace di
    {
        template <typename T>
        concept ModuleDependency = DerivedFromModule<T> && Dependency<T>;
    }
}

SOGE_DI_REGISTER_NS(soge, Module, df::Abstract<soge::Module>)

static_assert(soge::di::ModuleDependency<soge::Module> && soge::di::AbstractDependency<soge::Module>,
              "soge::Module should be an abstract module dependency");

#define SOGE_DI_REGISTER_MODULE_NS(ns, T, ...)                                                                         \
    SOGE_DI_REGISTER_NS(ns, T, df::External<ns::T>, tag::Overrides<ns::T, soge::Module>, __VA_ARGS__)

#define SOGE_DI_REGISTER_MODULE(T, ...) SOGE_DI_REGISTER_MODULE_NS(, T, __VA_ARGS__)

#endif // SOGE_CORE_MODULE_HPP
