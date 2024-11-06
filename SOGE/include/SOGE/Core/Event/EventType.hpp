#ifndef SOGE_CORE_EVENT_EVENTTYPE_HPP
#define SOGE_CORE_EVENT_EVENTTYPE_HPP

#include "SOGE/Core/Event/EventCategory.hpp"
#include "SOGE/System/StringId.hpp"


namespace soge
{
    class EventType
    {
    private:
        StringId m_id;
        EventCategory m_category;

    public:
        constexpr explicit EventType(const StringId& aId, EventCategory aCategory);

        [[nodiscard]]
        constexpr const StringId& GetId() const noexcept;
        [[nodiscard]]
        constexpr EventCategory GetCategory() const noexcept;

        constexpr auto operator<=>(const EventType&) const noexcept = default;
        constexpr bool operator==(const EventType&) const noexcept = default;
    };

    constexpr EventType::EventType(const StringId& aId, const EventCategory aCategory)
        : m_id(aId), m_category(aCategory)
    {
    }

    constexpr const StringId& EventType::GetId() const noexcept
    {
        return m_id;
    }

    constexpr EventCategory EventType::GetCategory() const noexcept
    {
        return m_category;
    }
}

#endif // SOGE_CORE_EVENT_EVENTTYPE_HPP
