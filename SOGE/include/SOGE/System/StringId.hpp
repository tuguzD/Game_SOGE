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
        using Set = eastl::hash_set<String, eastl::hash<String>, eastl::equal_to<String>, EASTLAllocatorType, true>;
        static Set s_set;

        Hash m_hash;
        View m_view;

    public:
        explicit StringId(View aView);
        explicit StringId(CStr aCStr);
        explicit StringId(std::nullptr_t) = delete;

        explicit constexpr StringId(const StringId&) noexcept = default;
        constexpr StringId& operator=(const StringId&) noexcept = default;

        explicit constexpr StringId(StringId&&) noexcept = default;
        constexpr StringId& operator=(StringId&&) noexcept = default;

        ~StringId() = default;

        [[nodiscard]]
        Hash GetHash() const noexcept;
        [[nodiscard]]
        View GetView() const noexcept;

        operator View() const noexcept;
        operator CStr() const noexcept;

        auto operator<=>(const StringId&) const noexcept = default;
        bool operator==(const StringId&) const noexcept = default;
    };
}

#endif // SOGE_SYSTEM_STRINGID_HPP
