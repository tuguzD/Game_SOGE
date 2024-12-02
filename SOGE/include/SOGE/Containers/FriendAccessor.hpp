#ifndef SOGE_CONTAINERS_FRIENDACCESSOR_HPP
#define SOGE_CONTAINERS_FRIENDACCESSOR_HPP

#include <EASTL/functional.h>


namespace soge
{
    template <typename ClassType, typename ReturnType, typename... Args>
    class FriendFuncPtr final
    {
        using MemberFunctionPtr = ReturnType(ClassType::*)(Args...);

    protected:
        MemberFunctionPtr m_funcPtr;

    public:
        explicit FriendFuncPtr(const MemberFunctionPtr& aFuncPtr) : m_funcPtr(aFuncPtr) {}

        template <typename AccessorType>
        friend class FriendAccessor;
    };

    template <typename TargetClass, typename ClassType, typename ReturnType, typename... Args>
    class FriendFuncAccessor final
    {
        using MemberFunctionPtr = ReturnType(ClassType::*)(Args...);

    private:
        MemberFunctionPtr m_funcPtr;

    public:
        explicit FriendFuncAccessor(const FriendFuncPtr<ClassType, ReturnType, Args...>& aFriendFuncPtr)
            : m_funcPtr(aFriendFuncPtr.m_funcPtr){};

        ReturnType Call(TargetClass& aTargetClass, Args&&... args) const {
            return (aTargetClass.*m_funcPtr)(std::forward<Args>(args)...);
        }
    };
}

#endif // !SOGE_CONTAINERS_FRIENDACCESSOR_HPP
