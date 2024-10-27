#pragma once

#include <tevents/EventKeyWrapper.hpp>

namespace te
{
    inline EventKeyWrapper::EventKeyWrapper(const void* objectPtr, std::shared_ptr<uint8_t[]> callbackPtr,
                                            const size_t callbackSize) : _objectPtr(objectPtr),
                                                                         _callbackPtr(std::move(callbackPtr)),
                                                                         _callbackSize(callbackSize)
    {
    }


    inline auto EventKeyWrapper::operator==(const EventKeyWrapper& other) const -> bool
    {
        return _objectPtr == other._objectPtr &&
               _callbackSize == other._callbackSize &&
               std::memcmp(_callbackPtr.get(), other._callbackPtr.get(), _callbackSize) == 0;
    }

    inline auto EventKeyWrapper::Hash::operator()(const EventKeyWrapper& eventKeyWrapper) const -> size_t
    {
        size_t callbackHash = 0;
        const auto* const callbackPtr = static_cast<const unsigned char *>(eventKeyWrapper._callbackPtr.get());
        for (size_t i = 0; i < eventKeyWrapper._callbackSize; i++)
        {
            callbackHash ^= static_cast<size_t>(callbackPtr[i]) << i % sizeof(size_t);
        }

        const auto instanceHash = std::hash<const void *>{}(eventKeyWrapper._objectPtr);
        return callbackHash ^ instanceHash << 1;
    }
}
