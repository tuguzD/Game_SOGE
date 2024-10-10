#ifndef SOGE_MEMORY_HPP
#define SOGE_MEMORY_HPP

#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/utility.h>


namespace soge
{
    template<typename _Ty>
    using sharedPtr = eastl::shared_ptr<_Ty>;
    template<typename _Ty, typename ..._Args>
    constexpr sharedPtr<_Ty> CreateShared(_Args&& ... args) {
        return eastl::make_shared<_Ty>(eastl::forward<_Args>(args)...);
    }

    template<typename _Ty>
    using uniquePtr = eastl::unique_ptr<_Ty>;
    template<typename _Ty, typename ..._Args>
    constexpr uniquePtr<_Ty> CreateUnique(_Args&& ... args) {
        return eastl::make_unique<_Ty>(eastl::forward<_Args>(args)...);
    }
}

#endif // !SOGE_MEMORY_HPP
