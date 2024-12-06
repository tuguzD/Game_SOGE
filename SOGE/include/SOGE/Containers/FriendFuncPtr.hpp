#ifndef SOGE_CONTAINERS_FRIENDFUNCPTR_HPP
#define SOGE_CONTAINERS_FRIENDFUNCPTR_HPP


namespace soge
{
    /*
     This is not autistic concept, it's just special, I promise.... Okay, maybe it is autistic, but let me explain
     something.

     We have some library/platform-agnostic shit in our codebase. For example, let's take the class called Keyboard.
     It is a pure virtual piece of dog shit which is the base of the implementation class called SDLKeyboard.
     Then, instead of having some logical function to decide what's the implementation we want to use, we just have
     compile-time macros which do it for us. Even so, we still need the implementation class name to operate with it,
     that's why we typedef implementation class at the end of the file to some standardized naming structure for this.

     For example the SDLKeyboard has the **typedef SDLKeyboard ImplKeyboard** thing.

     But we are still using Key types in the Keyboard implementation. So, when we are trying to make some friends with
     ImplKeyboard, compile count this as overriding of ImplKeyboard class and throws an error. That's why this weird
     thing has to be.

     Example of usage:

        // Target class with a protected function
        class Target {
        protected:
            std::string secretFunction(int value) {
                return "Value: " + std::to_string(value);
            }

        public:
            static FriendPtr<Target, std::string, int> getFriendPtr() {
                return FriendPtr<Target, std::string, int>(&Target::secretFunction);
            }
        };

        // Accessor usage
        class ExternalAccessor {
        public:
            std::string access(Target& target, int value) {
                // Explicit instantiation of Accessor
                Accessor<Target, std::string, int> accessor(Target::getFriendPtr());
                return accessor.Call(target, value);
            }
        };

        int main() {
            Target target;
            ExternalAccessor accessor;

            std::cout << accessor.access(target, 42) << std::endl; // Output: Value: 42
            return 0;
        }
    */

    template <typename ClassType, typename ReturnType, typename... Args>
    class FriendFuncAccessor;

    // ClassType - Type of class which protected method you want to use externally
    // ReturnType - Return type of the protected method
    // Args... - All argument types of protected method
    template <typename ClassType, typename ReturnType, typename... Args>
    class FriendFuncPtr final
    {
    private:
        friend FriendFuncAccessor<ClassType, ReturnType, Args...>;

        using MemberFunctionPtr = ReturnType (ClassType::*)(Args...);
        MemberFunctionPtr m_funcPtr;

    public:
        explicit FriendFuncPtr(const MemberFunctionPtr& aFuncPtr) : m_funcPtr(aFuncPtr)
        {
        }
    };

    // ClassType - the class of such protected method
    // ReturnType - return type of this protected method
    // Args... - Argument types of this protected method
    template <typename ClassType, typename ReturnType, typename... Args>
    class FriendFuncAccessor final
    {
    private:
        using FriendFuncPtr = FriendFuncPtr<ClassType, ReturnType, Args...>;
        typename FriendFuncPtr::MemberFunctionPtr m_funcPtr;

    public:
        explicit FriendFuncAccessor(const FriendFuncPtr& aFriendFuncPtr) : m_funcPtr(aFriendFuncPtr.m_funcPtr)
        {
        }

        ReturnType Call(ClassType& aTargetClass, Args&&... args) const
        {
            return (aTargetClass.*m_funcPtr)(std::forward<Args>(args)...);
        }
    };
}

#endif // SOGE_CONTAINERS_FRIENDFUNCPTR_HPP
