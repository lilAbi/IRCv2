#pragma once

#include "core/logger.h"
#include "eventHandler.h"

#include <mutex>
#include <typeindex>

//Queue events and dispatch them
class EventManager {
    using EventHandlerPtr       = std::shared_ptr<BaseEventFunctionHandler>;
    using EventHandlerVector    = std::vector<EventHandlerPtr>;
public:
    static EventManager& get();

    //fire off an event
    template<typename EventType>
    requires std::derived_from<EventType, Event>
    bool publish(std::shared_ptr<EventType> event);

     //register callback functions to be used for fired event
    template<typename T, typename EventType>
    requires std::derived_from<EventType, Event>
    void subscribe(T* instance, void (T::*onEventMemberFunction)(std::shared_ptr<EventType> event));

     //remove copy/move constructor/assignment operators
     EventManager(const EventManager&) = delete;
     EventManager(EventManager&&) = delete;
     EventManager& operator=(const EventManager&) = delete;
     EventManager& operator=(EventManager&&) = delete;

private:
    EventManager();
    ~EventManager() = default;

    //return a copy of the a EventHandlerVector without the expired objects
    template<typename EventType>
    EventHandlerVector take_snapshot();


private:
    Logger*                     m_logger = &Logger::get();
    //mutex to lock when adding a new subscriber
    mutable std::mutex          m_mutex;
    //each key in the map is an "Event Type" that has a value of a vector of subscribed "Event Handlers (callback functions)"
    std::flat_map< std::type_index, EventHandlerVector > m_subscribers;
};

template<typename EventType>
requires std::derived_from<EventType, Event>
bool EventManager::publish(std::shared_ptr<EventType> event) {
    int invoked = 0;
    const EventHandlerVector handlers_copy = take_snapshot<EventType>();
    for ( const EventHandlerPtr& handler : handlers_copy ) {
        invoked += handler->call(event) ? 1 : 0;
    }
    return invoked;
}

template<typename T, typename EventType>
requires std::derived_from<EventType, Event>
void EventManager::subscribe(T* instance, void(T::*onEventMemberFunction)(std::shared_ptr<EventType> event)) {
    std::lock_guard lock{m_mutex};
    auto handler = std::make_shared<MemberFunctionEventHandler<T, EventType>>(std::weak_ptr<T>{instance}, onEventMemberFunction);
    const std::type_index key{typeid(EventType)};
    m_subscribers.try_emplace(key).first->second.push_back(std::move(handler));
}

template<typename EventType>
EventManager::EventHandlerVector EventManager::take_snapshot() {
    std::lock_guard lock{m_mutex};
    if ( const auto itr = m_subscribers.find( std::type_index{typeid(EventType)} ); itr != m_subscribers.end() ) {
        EventHandlerVector& handlers = itr->second;
        std::erase_if(handlers, [](const EventHandlerPtr& handler) {
            return !handler || handler->expired();
        });
        return handlers;
    } else {
        return {};
    }
}
