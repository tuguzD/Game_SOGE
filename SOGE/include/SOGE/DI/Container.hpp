#ifndef SOGE_DI_CONTAINER_HPP
#define SOGE_DI_CONTAINER_HPP

#include "SOGE/DI/Provider.hpp"


namespace soge::di
{
    class Container : public Provider
    {
    public:
        template <Dependency T, typename... Args>
        bool Create(Args&&... args);

        template <Dependency T, typename... Args>
        void Recreate(Args&&... args);

        void Clear();
    };

    template <Dependency T, typename... Args>
    bool Container::Create(Args&&... args)
    {
        using Service = DependencyDefinition<T>;

        return m_container.emplace<Service>(std::forward<Args>(args)...);
    }

    template <Dependency T, typename... Args>
    void Container::Recreate(Args&&... args)
    {
        using Service = DependencyDefinition<T>;

        m_container.replace<Service>(std::forward<Args>(args)...);
    }

    inline void Container::Clear()
    {
        m_container.clear();
    }
}

#endif // SOGE_DI_CONTAINER_HPP
