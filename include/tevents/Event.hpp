#pragma once

#include <unordered_map>
#include <functional>

#include <tevents/EventKeyWrapper.hpp>

namespace te
{
    template<typename... Args>
    class Event
    {
    public:
        void Subscribe(void(&callback)(Args...));
        template<typename T> void Subscribe(T *instance, void(T::*callback)(Args...));
        template<typename T> void Subscribe(T &instance, void(T::*callback)(Args...));

        void Unsubscribe(void(&callback)(Args...));
        template<typename T> void Unsubscribe(T *instance, void(T::*callback)(Args...));
        template<typename T> void Unsubscribe(T &instance, void(T::*callback)(Args...));

        void operator()(Args... args) const;
        void Invoke(Args... args) const;

        void Clear();

    private:
        EventKeyWrapper MakeKey(void(*callback)(Args...));
        template<typename T> EventKeyWrapper MakeKey(T *instance, void(T::*callback)(Args...));

        std::unordered_map<EventKeyWrapper, std::function<void(Args...)>, EventKeyWrapper::Hash> _callbacks;
    };
}