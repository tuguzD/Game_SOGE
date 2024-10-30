#ifndef SOGE_CORE_EVENT_EVENTCATEGORY_HPP
#define SOGE_CORE_EVENT_EVENTCATEGORY_HPP

#include <variant>


namespace soge
{
    class EventCategory
    {
    public:
        struct Default
        {
            constexpr auto operator<=>(const Default&) const noexcept = default;
            constexpr bool operator==(const Default&) const noexcept = default;
        };

        struct Input
        {
            constexpr auto operator<=>(const Input&) const noexcept = default;
            constexpr bool operator==(const Input&) const noexcept = default;
        };

        class Custom
        {
        private:
            using Priority = std::uint32_t;
            Priority m_priority;

        public:
            explicit constexpr Custom(Priority aPriority) noexcept;

            explicit constexpr operator Priority() const noexcept;

            constexpr auto operator<=>(const Custom&) const noexcept = default;
            constexpr bool operator==(const Custom&) const noexcept = default;
        };

    private:
        using Variants = std::variant<Default, Input, Custom>;
        Variants m_variants;

    public:
        explicit constexpr EventCategory() noexcept = default;
        constexpr EventCategory(const Variants& aVariants) noexcept;

        template <typename T>
        constexpr EventCategory(T&& aVariant) noexcept
        requires std::constructible_from<Variants, T>;

        [[nodiscard]]
        constexpr bool IsCustom() const noexcept;

        constexpr auto operator<=>(const EventCategory&) const noexcept = default;
        constexpr bool operator==(const EventCategory&) const noexcept = default;
    };

    constexpr EventCategory::EventCategory(const Variants& aVariants) noexcept : m_variants(aVariants)
    {
    }

    constexpr bool EventCategory::IsCustom() const noexcept
    {
        return std::holds_alternative<Custom>(m_variants);
    }

    template <typename T>
    constexpr EventCategory::EventCategory(T&& aVariant) noexcept
    requires std::constructible_from<Variants, T>
        : m_variants(std::forward<T>(aVariant))
    {
    }

    constexpr EventCategory::Custom::Custom(const Priority aPriority) noexcept : m_priority(aPriority)
    {
    }

    constexpr EventCategory::Custom::operator Priority() const noexcept
    {
        return m_priority;
    }
}

#endif // SOGE_CORE_EVENT_EVENTCATEGORY_HPP
