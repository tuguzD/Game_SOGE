#include "sogepch.hpp"

#include "SOGE/System/StringId.hpp"


namespace soge
{
    StringId::Set StringId::s_set;

    void StringId::InitializeAtRuntime(StrView aView)
    {
        eastl::string_view view{aView.data(), aView.size()};
        m_hash = eastl::hash<decltype(view)>{}(view);

        auto hash = [&](const auto&) { return m_hash; };
        if (const auto iter = s_set.find_as(view, hash, eastl::equal_to()); iter != s_set.end())
        {
            m_view = StrView{iter->data(), iter->size()};
            return;
        }

        const auto iter = s_set.emplace(view).first;
        m_view = StrView{iter->data(), iter->size()};
    }
}
