#pragma once

#include <cstring>

#include <tevents/Event.hpp>

namespace te
{
    template<typename... Args>
    void Event<Args...>::Subscribe(void(&callback)(Args...))
    {
        auto key = MakeKey(callback);
        auto packedCallback = std::function(callback);

        _callbacks.insert({key, packedCallback});
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Subscribe(T *instance, void(T::*callback)(Args...))
    {
        auto key = MakeKey(instance, callback);
        auto packedCallback = std::function(
                [instance, callback](Args &&...args)
                {
                    (instance->*callback)(args...);
                });

        _callbacks.insert({key, packedCallback});
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Subscribe(T &instance, void(T::*callback)(Args...))
    {
        Subscribe(&instance, callback);
    }

    template<typename... Args>
    void Event<Args...>::Unsubscribe(void(&callback)(Args...))
    {
        auto key = MakeKey(callback);
        _callbacks.erase(key);
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Unsubscribe(T *instance, void(T::*callback)(Args...))
    {
        auto key = MakeKey(instance, callback);
        _callbacks.erase(key);
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Unsubscribe(T &instance, void(T::*callback)(Args...))
    {
        Unsubscribe(&instance, callback);
    }

    template<typename... Args>
    void Event<Args...>::operator()(Args... args) const
    {
        Invoke(args...);
    }

    template<typename... Args>
    void Event<Args...>::Invoke(Args... args) const
    {
        for (auto callbackPair: _callbacks)
        {
            callbackPair.second(args...);
        }
    }

    template<typename... Args>
    EventKeyWrapper Event<Args...>::MakeKey(void(*callback)(Args...))
    {
        auto callbackSize = sizeof(&callback);
        std::shared_ptr<uint8_t[]> callbackPtr(new uint8_t[callbackSize]);
        std::memcpy(callbackPtr.get(), (void *) &callback, callbackSize);
        return EventKeyWrapper(nullptr, callbackPtr, callbackSize);
    }

    template<typename... Args>
    template<typename T>
    EventKeyWrapper Event<Args...>::MakeKey(T *instance, void(T::*callback)(Args...))
    {
        auto callbackSize = sizeof(callback);
        std::shared_ptr<uint8_t[]> callbackPtr(new uint8_t[callbackSize]);
        std::memcpy(callbackPtr.get(), (void *) &callback, callbackSize);
        return EventKeyWrapper(instance, callbackPtr, callbackSize);
    }
}