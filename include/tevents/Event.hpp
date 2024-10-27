#pragma once

#include <tevents/EventKeyWrapper.hpp>

namespace te
{
    template<typename... Args>
    class Event
    {
    public:
        void Subscribe(void (&callback)(Args...));
        template<typename T>void Subscribe(T* instance, void (T::*callback)(Args...));
        template<typename T>void Subscribe(T& instance, void (T::*callback)(Args...));
        template<typename T>void Subscribe(const T* instance, void (T::*callback)(Args...) const);
        template<typename T>void Subscribe(const T& instance, void (T::*callback)(Args...) const);

        void Unsubscribe(void (&callback)(Args...));
        template<typename T>void Unsubscribe(T* instance, void (T::*callback)(Args...));
        template<typename T>void Unsubscribe(T& instance, void (T::*callback)(Args...));
        template<typename T>void Unsubscribe(const T* instance, void (T::*callback)(Args...) const);
        template<typename T>void Unsubscribe(const T& instance, void (T::*callback)(Args...) const);

        void Invoke(Args... args) const;

        void operator()(Args... args) const;

        void Clear();

    private:
        void InsertCallback(EventKeyWrapper key, std::function<void(Args...)> callback);
        static auto MakeKey(void (*callback)(Args...)) -> EventKeyWrapper;
        template<typename T>static auto MakeKey(T* instance, void (T::*callback)(Args...)) -> EventKeyWrapper;
        template<typename T>static auto MakeKey(const T* instance, void (T::*callback)(Args...) const) -> EventKeyWrapper;
        template<typename T>static auto MakeKey(T* instance, const void* callback, std::size_t callbackSize) -> EventKeyWrapper;

        std::unordered_map<EventKeyWrapper, std::function<void(Args...)>, EventKeyWrapper::Hash> _callbacks;
    };
}
