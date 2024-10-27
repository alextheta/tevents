#pragma once

#include <functional>
#include <tevents/Event.hpp>

namespace te
{
    template<typename... Args>
    void Event<Args...>::Subscribe(void (&callback)(Args...))
    {
        InsertCallback(MakeKey(callback), callback);
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Subscribe(T* instance, void (T::*callback)(Args...))
    {
        InsertCallback(MakeKey(instance, callback), [instance, callback](Args&&... args)
        {
            (instance->*callback)(std::forward<Args>(args)...);
        });
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Subscribe(T& instance, void (T::*callback)(Args...))
    {
        Subscribe(&instance, callback);
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Subscribe(const T* instance, void (T::*callback)(Args...) const)
    {
        InsertCallback(MakeKey(instance, callback), [instance, callback](Args&&... args)
        {
            (instance->*callback)(std::forward<Args>(args)...);
        });
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Subscribe(const T& instance, void (T::*callback)(Args...) const)
    {
        Subscribe(&instance, callback);
    }

    template<typename... Args>
    void Event<Args...>::Unsubscribe(void (&callback)(Args...))
    {
        _callbacks.erase(MakeKey(callback));
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Unsubscribe(T* instance, void (T::*callback)(Args...))
    {
        _callbacks.erase(MakeKey(instance, callback));
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Unsubscribe(T& instance, void (T::*callback)(Args...))
    {
        Unsubscribe(&instance, callback);
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Unsubscribe(const T* instance, void (T::*callback)(Args...) const)
    {
        _callbacks.erase(MakeKey(instance, callback));
    }

    template<typename... Args>
    template<typename T>
    void Event<Args...>::Unsubscribe(const T& instance, void (T::*callback)(Args...) const)
    {
        Unsubscribe(&instance, callback);
    }

    template<typename... Args>
    void Event<Args...>::Invoke(Args... args) const
    {
        for (const auto& callbackPair: _callbacks)
        {
            callbackPair.second(args...);
        }
    }

    template<typename... Args>
    void Event<Args...>::operator()(Args... args) const
    {
        Invoke(args...);
    }

    template<typename... Args>
    void Event<Args...>::Clear()
    {
        _callbacks.clear();
    }

    template<typename... Args>
    void Event<Args...>::InsertCallback(EventKeyWrapper key, std::function<void(Args...)> callback)
    {
        _callbacks.insert({key, std::move(callback)});
    }

    template<typename... Args>
    auto Event<Args...>::MakeKey(void (*callback)(Args...)) -> EventKeyWrapper
    {
        return MakeKey(nullptr, &callback, sizeof(callback));
    }

    template<typename... Args>
    template<typename T>
    auto Event<Args...>::MakeKey(T* instance, void (T::*callback)(Args...)) -> EventKeyWrapper
    {
        return MakeKey(instance, &callback, sizeof(callback));
    }

    template<typename... Args>
    template<typename T>
    auto Event<Args...>::MakeKey(const T* instance, void (T::*callback)(Args...) const) -> EventKeyWrapper
    {
        return MakeKey(instance, &callback, sizeof(callback));
    }

    template<typename... Args>
    template<typename T>
    auto Event<Args...>::MakeKey(T* instance, const void* callback, std::size_t callbackSize) -> EventKeyWrapper
    {
        std::shared_ptr<uint8_t[]> callbackPtr(new uint8_t[callbackSize]);
        std::memcpy(callbackPtr.get(), callback, callbackSize);
        return EventKeyWrapper(instance, std::move(callbackPtr), callbackSize);
    }
}
