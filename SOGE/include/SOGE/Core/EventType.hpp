#ifndef SOGE_CORE_EVENTTYPE_HPP
#define SOGE_CORE_EVENTTYPE_HPP

#include "SOGE/System/StringId.hpp"


namespace soge
{
    class EventType final
    {
    private:
        StringId m_id;

    public:
        constexpr explicit EventType(const StringId& aId);

        [[nodiscard]]
        constexpr const StringId& GetId() const noexcept;
    };

    constexpr EventType::EventType(const StringId& aId) : m_id(aId)
    {
    }

    constexpr const StringId& EventType::GetId() const noexcept
    {
        return m_id;
    }
}

#endif // SOGE_CORE_EVENTTYPE_HPP
