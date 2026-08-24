#pragma once

#include <functional>
#include <memory>

class Event;

class BaseEventFunctionHandler{
public:
    virtual ~BaseEventFunctionHandler() = default;
    //Returns false when the receiver no longer exists.
    virtual bool call(const Event& event) = 0;
    virtual bool expired() = 0;
};

template<typename T, typename EventType>
class MemberFunctionEventHandler : public BaseEventFunctionHandler {
    using OnEventMemberFunction = void(T::*)(std::shared_ptr<EventType>);
public:
    MemberFunctionEventHandler(std::weak_ptr<T> instance, const OnEventMemberFunction func) : m_instance(instance), m_callback(func) {};

    bool call(const Event& event) override;
    bool expired() override;

private:
    std::weak_ptr<T>        m_instance;
    OnEventMemberFunction   m_callback;
};

template<typename T, typename EventType>
bool MemberFunctionEventHandler<T, EventType>::call(const Event& event) {
    if (auto instance = m_instance.lock()) {
        return false;
    } else {
        std::invoke(m_callback, *m_instance, static_cast<const EventType&>(event));
        return true;
    }
}

template<typename T, typename EventType>
bool MemberFunctionEventHandler<T, EventType>::expired() {
    return m_instance.expired();
}