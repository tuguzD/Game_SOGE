#ifndef SOGE_CORE_EVENT_EVENTCHAIN_HPP
#define SOGE_CORE_EVENT_EVENTCHAIN_HPP

#include "SOGE/Core/Event/Event.hpp"

#include <eventpp/callbacklist.h>


namespace soge
{
    template <DerivedFromEvent T>
    class EventChain
    {
    private:
        class Policies
        {
        private:
            explicit Policies() = default;

        public:
            // NOLINTNEXTLINE(readability-identifier-naming) reason: required by `eventpp` library
            static constexpr bool canContinueInvoking(const T& aEvent);
        };

        using FunctionList = eventpp::CallbackList<bool(T&), Policies>;
        FunctionList m_functions;

    public:
        using Function = typename FunctionList::Callback;
        class FunctionHandle;

        explicit EventChain() noexcept = default;
        ~EventChain() = default;

        explicit EventChain(const EventChain&) = default;
        EventChain& operator=(const EventChain&) = default;

        explicit EventChain(EventChain&&) noexcept = default;
        EventChain& operator=(EventChain&&) noexcept = default;

        [[nodiscard]]
        bool IsEmpty() const;

        template <typename F>
        requires std::is_invocable_r_v<bool, F, T&>
        FunctionHandle PushBack(F&& aFunction);

        template <typename F>
        requires std::is_invocable_r_v<bool, F, T&>
        FunctionHandle PushFront(F&& aFunction);

        template <typename F>
        requires std::is_invocable_r_v<bool, F, T&>
        FunctionHandle Insert(F&& aFunction, const FunctionHandle& aBefore);

        bool Remove(const FunctionHandle& aHandle);

        [[nodiscard]]
        bool Contains(const FunctionHandle& aHandle) const;

        void Clear();

        bool operator()(T& aEvent) const;

        template <typename F>
        requires std::is_invocable_r_v<bool, F, T&>
        EventChain& operator+=(F&& aFunction);

        EventChain& operator-=(const FunctionHandle& aHandle);
    };

    template <DerivedFromEvent T>
    class EventChain<T>::FunctionHandle : public FunctionList::Handle
    {
    private:
        using Parent = typename FunctionList::Handle;
        friend EventChain;

        using Parent::Parent;
        explicit FunctionHandle(const Parent& aHandle);

    public:
        [[nodiscard]]
        FunctionHandle GetPrevious() const;

        [[nodiscard]]
        FunctionHandle GetNext() const;
    };

    template <DerivedFromEvent T>
    constexpr bool EventChain<T>::Policies::canContinueInvoking(const T& aEvent)
    {
        return !aEvent.IsHandled();
    }

    template <DerivedFromEvent T>
    bool EventChain<T>::IsEmpty() const
    {
        return m_functions.empty();
    }

    template <DerivedFromEvent T>
    template <typename F>
    requires std::is_invocable_r_v<bool, F, T&>
    auto EventChain<T>::PushBack(F&& aFunction) -> FunctionHandle
    {
        auto handle = m_functions.append(std::forward<F>(aFunction));
        return FunctionHandle{handle};
    }

    template <DerivedFromEvent T>
    template <typename F>
    requires std::is_invocable_r_v<bool, F, T&>
    auto EventChain<T>::PushFront(F&& aFunction) -> FunctionHandle
    {
        auto handle = m_functions.prepend(std::forward<F>(aFunction));
        return FunctionHandle{handle};
    }

    template <DerivedFromEvent T>
    template <typename F>
    requires std::is_invocable_r_v<bool, F, T&>
    auto EventChain<T>::Insert(F&& aFunction, const FunctionHandle& aBefore) -> FunctionHandle
    {
        if (!m_functions.ownsHandle(aBefore))
        {
            auto handle = m_functions.append(std::forward<F>(aFunction));
            return FunctionHandle{handle};
        }
        auto handle = m_functions.insert(std::forward<F>(aFunction), aBefore);
        return FunctionHandle{handle};
    }

    template <DerivedFromEvent T>
    bool EventChain<T>::Remove(const FunctionHandle& aHandle)
    {
        if (!m_functions.ownsHandle(aHandle))
        {
            return false;
        }
        return m_functions.remove(aHandle);
    }

    template <DerivedFromEvent T>
    bool EventChain<T>::Contains(const FunctionHandle& aHandle) const
    {
        return m_functions.ownsHandle(aHandle);
    }

    template <DerivedFromEvent T>
    void EventChain<T>::Clear()
    {
        FunctionList functions;
        m_functions.swap(functions);
    }

    template <DerivedFromEvent T>
    bool EventChain<T>::operator()(T& aEvent) const
    {
        return m_functions.forEachIf([&aEvent](Function& aFunction) {
            bool result = aFunction(aEvent);
            return result && !aEvent.IsHandled();
        });
    }

    template <DerivedFromEvent T>
    template <typename F>
    requires std::is_invocable_r_v<bool, F, T&>
    auto EventChain<T>::operator+=(F&& aFunction) -> EventChain&
    {
        PushBack(std::forward<F>(aFunction));
        return *this;
    }

    template <DerivedFromEvent T>
    auto EventChain<T>::operator-=(const FunctionHandle& aHandle) -> EventChain&
    {
        Remove(aHandle);
        return *this;
    }

    template <DerivedFromEvent T>
    EventChain<T>::FunctionHandle::FunctionHandle(const Parent& aHandle) : Parent(aHandle)
    {
    }

    template <DerivedFromEvent T>
    auto EventChain<T>::FunctionHandle::GetPrevious() const -> FunctionHandle
    {
        const std::shared_ptr node = this->lock();
        return node != nullptr ? FunctionHandle{node->previous} : FunctionHandle{};
    }

    template <DerivedFromEvent T>
    auto EventChain<T>::FunctionHandle::GetNext() const -> FunctionHandle
    {
        const std::shared_ptr node = this->lock();
        return node != nullptr ? FunctionHandle{node->next} : FunctionHandle{};
    }
}

#endif // SOGE_CORE_EVENT_EVENTCHAIN_HPP
