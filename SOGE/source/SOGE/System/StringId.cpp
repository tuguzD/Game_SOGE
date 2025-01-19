#include "sogepch.hpp"

#include "SOGE/System/StringId.hpp"


namespace soge
{
    StringId::Set StringId::s_set;
    std::mutex StringId::s_mutex;

    auto StringId::Hasher::operator()(const String& aStr) const noexcept -> Hash
    {
        const StrView view{aStr.data(), aStr.size()};
        return operator()(view);
    }

    void StringId::InitializeAtRuntime(StrView aView)
    {
        eastl::string_view view{aView.data(), aView.size()};
        m_hash = s_set.hash_function()(view);

        // Try to be thread-safe and to not invalidate iterators
        std::lock_guard lock{s_mutex};

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
