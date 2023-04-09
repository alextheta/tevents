#pragma once

#include <string>
#include <tevents/EventKeyWrapper.hpp>

namespace te
{
    inline EventKeyWrapper::EventKeyWrapper(void *objectPtr, std::shared_ptr<uint8_t[]> callbackPtr, size_t callbackSize) :
            _objectPtr(objectPtr),
            _callbackPtr(callbackPtr),
            _callbackSize(callbackSize)
    {
    }

    inline bool EventKeyWrapper::operator==(const EventKeyWrapper &other) const
    {
        return other._objectPtr == _objectPtr &&
               other._callbackSize == _callbackSize &&
               std::memcmp(other._callbackPtr.get(), _callbackPtr.get(), _callbackSize) == 0;
    }

    inline size_t EventKeyWrapper::Hash::operator()(const EventKeyWrapper &eventKeyWrapper) const
    {
        std::string s((char *) eventKeyWrapper._callbackPtr.get(), eventKeyWrapper._callbackSize);
        auto h1 = std::hash<std::string>{}(s);
        auto h2 = std::hash<void *>{}(eventKeyWrapper._objectPtr);
        return h1 ^ (h2 << 1);
    }
}