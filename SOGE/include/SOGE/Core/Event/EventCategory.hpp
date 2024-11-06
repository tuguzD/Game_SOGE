#ifndef SOGE_CORE_EVENT_EVENTCATEGORY_HPP
#define SOGE_CORE_EVENT_EVENTCATEGORY_HPP

#include <variant>


namespace soge
{
    class EventCategory
    {
    public:
        enum class Predefined : std::uint8_t
        {
            Default,
            Core,
            Input,
        };
        using enum Predefined;

        class Custom
        {
        public:
            using Id = std::uint32_t;

            explicit constexpr Custom(Id aId) noexcept;

            [[nodiscard]]
            constexpr Id GetId() const noexcept;

            explicit constexpr operator Id() const noexcept;

            constexpr auto operator<=>(const Custom&) const noexcept = default;
            constexpr bool operator==(const Custom&) const noexcept = default;

        private:
            Id m_priority;
        };

    private:
        friend std::hash<EventCategory>;
        friend eastl::hash<EventCategory>;

        using Variants = std::variant<Predefined, Custom>;
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

    constexpr EventCategory::Custom::Custom(const Id aId) noexcept : m_priority(aId)
    {
    }

    constexpr auto EventCategory::Custom::GetId() const noexcept -> Id
    {
        return m_priority;
    }

    constexpr EventCategory::Custom::operator Id() const noexcept
    {
        return m_priority;
    }
}

template <>
struct std::hash<soge::EventCategory::Custom>
{
    constexpr std::size_t operator()(const soge::EventCategory::Custom& aCustom) const noexcept
    {
        return aCustom.GetId();
    }
};

template <>
struct eastl::hash<soge::EventCategory::Custom>
{
    constexpr eastl_size_t operator()(const soge::EventCategory::Custom& aCustom) const noexcept
    {
        return aCustom.GetId();
    }
};

template <>
struct std::hash<soge::EventCategory>
{
    std::size_t operator()(const soge::EventCategory& aCategory) const noexcept
    {
        constexpr std::hash<soge::EventCategory::Variants> hasher{};
        return hasher(aCategory.m_variants);
    }
};

template <>
struct eastl::hash<soge::EventCategory>
{
    eastl_size_t operator()(const soge::EventCategory& aCategory) const noexcept
    {
        constexpr std::hash<soge::EventCategory::Variants> hasher{};
        return hasher(aCategory.m_variants);
    }
};

#endif // SOGE_CORE_EVENT_EVENTCATEGORY_HPP
