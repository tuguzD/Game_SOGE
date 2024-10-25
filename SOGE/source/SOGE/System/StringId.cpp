#include "sogepch.hpp"

#include "SOGE/System/StringId.hpp"


namespace soge
{
    StringId::StringId(const View aView) : StringId(aView.data())
    {
    }

    StringId::StringId(CStr aCStr)
    {
        m_hash = eastl::hash<CStr>{}(aCStr);

        auto hash = [&](const auto&) { return m_hash; };
        if (const auto iter = s_set.find_as(aCStr, hash, eastl::equal_to()); iter != s_set.end())
        {
            m_view = {iter->data(), iter->size()};
            return;
        }

        const auto iter = s_set.emplace(aCStr).first;
        m_view = {iter->data(), iter->size()};
    }

    auto StringId::GetHash() const noexcept -> Hash
    {
        return m_hash;
    }

    auto StringId::GetView() const noexcept -> View
    {
        return m_view;
    }

    StringId::operator View() const noexcept
    {
        return m_view;
    }

    StringId::operator CStr() const noexcept
    {
        return m_view.data();
    }
}
