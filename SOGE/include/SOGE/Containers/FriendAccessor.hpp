#ifndef SOGE_CONTAINERS_FRIENDACCESSOR_HPP
#define SOGE_CONTAINERS_FRIENDACCESSOR_HPP


namespace soge
{
    // Fuck friending classes, all my homies using FrendFunctionPtr!
    // This is not an autistic concept actually, just special one. I'm promise.

    template <typename ClassType, typename ReturnType, typename... Args>
    class FriendFuncPtr final
    {
        using MemberFunctionPtr = ReturnType(ClassType::*)(Args...);

    protected:

    public:
        MemberFunctionPtr m_funcPtr;
        explicit FriendFuncPtr(const MemberFunctionPtr& aFuncPtr) : m_funcPtr(aFuncPtr) {}

    };

    template <typename TargetClass, typename ClassType, typename ReturnType, typename... Args>
    class FriendFuncAccessor final
    {
        using MemberFunctionPtr = ReturnType (ClassType::*)(Args...);

    private:
        MemberFunctionPtr m_funcPtr;

    public:
        explicit FriendFuncAccessor(const FriendFuncPtr<ClassType, ReturnType, Args...>& aFriendFuncPtr)
            : m_funcPtr(aFriendFuncPtr.m_funcPtr){};

        ReturnType Call(TargetClass& aTargetClass, Args&&... args) const
        {
            return (aTargetClass.*m_funcPtr)(std::forward<Args>(args)...);
        }
    };
}

#endif // !SOGE_CONTAINERS_FRIENDACCESSOR_HPP
