#ifndef SOGE_DI_PROVIDER_HPP
#define SOGE_DI_PROVIDER_HPP

#include "SOGE/DI/Debug.hpp"
#include "SOGE/DI/Dependency.hpp"

#include <kangaru/container.hpp>
#include <kangaru/operator_service.hpp>


namespace soge::di
{
    class Provider
    {
    private:
        friend class Container;

        kgr::container m_container;

    public:
        template <Dependency T>
        [[nodiscard]]
        auto Provide() -> decltype(auto);

        template <Dependency T>
        [[nodiscard]]
        Lazy<T> ProvideLazy();

        template <PolymorphicDependency T>
        [[nodiscard]]
        PolymorphicRange<T> ProvideRange();
    };

    template <Dependency T>
    auto Provider::Provide() -> decltype(auto)
    {
        using Service = DependencyDefinition<T>;

        debug::Provide<T>();
        return m_container.service<Service>();
    }

    template <Dependency T>
    Lazy<T> Provider::ProvideLazy()
    {
        using Service = kgr::lazy_service<DependencyDefinition<T>>;

        debug::Provide<T>();
        return m_container.service<Service>();
    }

    template <PolymorphicDependency T>
    PolymorphicRange<T> Provider::ProvideRange()
    {
        using Service = kgr::override_range_service<DependencyDefinition<T>>;

        debug::Provide<T>();
        return m_container.service<Service>();
    }
}

#endif // SOGE_DI_PROVIDER_HPP
