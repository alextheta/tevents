#pragma once

#include <memory>

namespace te
{
    class EventKeyWrapper
    {
    public:
        struct Hash
        {
            size_t operator()(const EventKeyWrapper &eventKeyWrapper) const;
        };

        EventKeyWrapper() = delete;
        EventKeyWrapper(void *objectPtr, std::shared_ptr<uint8_t[]> callbackPtr, size_t callbackSize);

        bool operator==(const EventKeyWrapper &other) const;

    private:
        void *_objectPtr;
        std::shared_ptr<uint8_t[]> _callbackPtr;
        size_t _callbackSize;
    };
}