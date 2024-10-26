#ifndef SOGE_SYSTEM_STRINGID_HPP
#define SOGE_SYSTEM_STRINGID_HPP

#include <EASTL/hash_set.h>
#include <EASTL/string.h>


namespace soge
{
    class StringId final
    {
    public:
        using Hash = eastl_size_t;
        using View = std::string_view;
        using CStr = View::const_pointer;

    private:
        using String = eastl::string;
        static_assert(eastl::is_same_v<CStr, String::const_pointer>,
                      "C-string and String must have the same char type");

        // Define separate hasher to use constexpr
        struct Hasher
        {
            constexpr Hash operator()(View aView) const noexcept;
        };

        using Set = eastl::hash_set<String, eastl::hash<String>, eastl::equal_to<String>, EASTLAllocatorType, true>;
        static Set s_set;

        void InitializeAtRuntime(View aView);

        Hash m_hash;
        View m_view;

    public:
        explicit constexpr StringId(View aView);
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
        constexpr View GetView() const noexcept;

        constexpr operator View() const noexcept;
        constexpr operator CStr() const noexcept;

        constexpr auto operator<=>(const StringId&) const noexcept = default;
        constexpr bool operator==(const StringId&) const noexcept = default;
    };

    namespace string_id_literals
    {
        constexpr StringId operator""_sid(const StringId::CStr aCStr, const std::size_t aLength)
        {
            const StringId::View view{aCStr, aLength};
            return StringId(view);
        }
    }

    // algorithm were taken from EASTL/string_view.h (at line 605)
    constexpr auto StringId::Hasher::operator()(const View aView) const noexcept -> Hash
    {
        View::const_iterator p = aView.cbegin();
        View::const_iterator end = aView.cend();
        uint32_t result = 2166136261U; // We implement an FNV-like string hash.
        while (p != end)
            result = (result * 16777619) ^ (uint8_t)*p++;
        return (size_t)result;
    }

    constexpr StringId::StringId(const View aView)
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

    constexpr StringId::StringId(const CStr aCStr) : StringId(View{aCStr})
    {
    }

    constexpr auto StringId::GetHash() const noexcept -> Hash
    {
        return m_hash;
    }

    constexpr auto StringId::GetView() const noexcept -> View
    {
        return m_view;
    }

    constexpr StringId::operator View() const noexcept
    {
        return m_view;
    }

    constexpr StringId::operator CStr() const noexcept
    {
        return m_view.data();
    }
}

#endif // SOGE_SYSTEM_STRINGID_HPP
