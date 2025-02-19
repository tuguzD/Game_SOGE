#ifndef SOGE_SYSTEM_STRINGID_HPP
#define SOGE_SYSTEM_STRINGID_HPP

#include <EASTL/hash_set.h>
#include <EASTL/string.h>

#include <mutex>


namespace soge
{
    class StringId final
    {
    public:
        using Hash = eastl_size_t;
        using StrView = std::string_view;
        using CStr = StrView::const_pointer;

    private:
        using String = eastl::string;
        static_assert(eastl::is_same_v<CStr, String::const_pointer>,
                      "C-string and String must have the same base char type");

        struct Hasher
        {
            constexpr Hash operator()(StrView aView) const noexcept;
            constexpr Hash operator()(eastl::string_view aView) const noexcept;
            Hash operator()(const String& aStr) const noexcept;
        };

        using Set = eastl::hash_set<String, Hasher, eastl::equal_to<String>, EASTLAllocatorType, true>;
        static Set s_set;
        static std::mutex s_mutex;

        void InitializeAtRuntime(StrView aView);

        Hash m_hash;
        StrView m_view;

    public:
        explicit constexpr StringId(StrView aView);
        explicit constexpr StringId(CStr aCStr);
        explicit constexpr StringId(std::nullptr_t) = delete;

        explicit constexpr StringId(const StringId&) noexcept = default;
        constexpr StringId& operator=(const StringId&) noexcept = default;

        explicit constexpr StringId(StringId&&) noexcept = default;
        constexpr StringId& operator=(StringId&&) noexcept = default;

        constexpr ~StringId() noexcept = default;

        [[nodiscard]]
        constexpr Hash GetHash() const noexcept;
        [[nodiscard]]
        constexpr StrView GetString() const noexcept;

        constexpr operator StrView() const noexcept;
        constexpr operator CStr() const noexcept;

        constexpr void Swap(StringId& aOther) noexcept;

        constexpr auto operator<=>(const StringId&) const noexcept = default;
        constexpr bool operator==(const StringId&) const noexcept = default;
    };

    namespace string_id_literals
    {
        constexpr StringId operator""_sid(const StringId::CStr aCStr, const std::size_t aLength)
        {
            const StringId::StrView view{aCStr, aLength};
            return StringId(view);
        }
    }

    // algorithm were taken from EASTL/string_view.h (at line 605)
    constexpr auto StringId::Hasher::operator()(const StrView aView) const noexcept -> Hash
    {
        StrView::const_iterator p = aView.cbegin();
        StrView::const_iterator end = aView.cend();
        uint32_t result = 2166136261U; // We implement an FNV-like string hash.
        while (p != end)
            result = (result * 16777619) ^ (uint8_t)*p++;
        return (size_t)result;
    }

    constexpr auto StringId::Hasher::operator()(const eastl::string_view aView) const noexcept -> Hash
    {
        const StrView view{aView.data(), aView.size()};
        return operator()(view);
    }

    constexpr StringId::StringId(const StrView aView)
    {
        if (std::is_constant_evaluated())
        {
            // TODO decide if we should save this string in the set
            // (but I guess this is not possible at compile time)
            m_hash = Hasher{}(aView);
            m_view = aView;
            return;
        }

        InitializeAtRuntime(aView);
    }

    constexpr StringId::StringId(const CStr aCStr) : StringId(StrView{aCStr})
    {
    }

    constexpr auto StringId::GetHash() const noexcept -> Hash
    {
        return m_hash;
    }

    constexpr auto StringId::GetString() const noexcept -> StrView
    {
        return m_view;
    }

    constexpr StringId::operator StrView() const noexcept
    {
        return m_view;
    }

    constexpr StringId::operator CStr() const noexcept
    {
        return m_view.data();
    }

    constexpr void StringId::Swap(StringId& aOther) noexcept
    {
        std::swap(m_hash, aOther.m_hash);
        std::swap(m_view, aOther.m_view);
    }
}

template <>
struct std::hash<soge::StringId>
{
    constexpr std::size_t operator()(const soge::StringId& aStringId) const noexcept
    {
        return aStringId.GetHash();
    }
};

template <>
struct eastl::hash<soge::StringId>
{
    constexpr eastl_size_t operator()(const soge::StringId& aStringId) const noexcept
    {
        return aStringId.GetHash();
    }
};

#endif // SOGE_SYSTEM_STRINGID_HPP
