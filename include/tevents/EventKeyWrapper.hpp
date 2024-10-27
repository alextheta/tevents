#pragma once

namespace te
{
    class EventKeyWrapper
    {
    public:
        struct Hash
        {
            auto operator()(const EventKeyWrapper& eventKeyWrapper) const -> size_t;
        };

        EventKeyWrapper() = delete;
        EventKeyWrapper(const void* objectPtr, std::shared_ptr<uint8_t[]> callbackPtr, size_t callbackSize);

        auto operator==(const EventKeyWrapper& other) const -> bool;

    private:
        const void* _objectPtr;
        std::shared_ptr<uint8_t[]> _callbackPtr;
        size_t _callbackSize;
    };
}
