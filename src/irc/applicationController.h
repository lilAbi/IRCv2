#pragma once
#include "ircMetadata.h"
#include "ircViewModel.h"
#include "container/threadSafeQueue.h"
#include "core/logger.h"

class ApplicationController {
public:
    ApplicationController(IrcViewModel& view_model, ThreadSafeQueue<NetworkEventVariant>& network_event_queue);
    //delete copy/move assignment/constructors
    ApplicationController(const ApplicationController&) = delete;
    ApplicationController(ApplicationController&&) = delete;
    ApplicationController& operator=(const ApplicationController&) = delete;
    ApplicationController& operator=(ApplicationController&&) = delete;
    //execute our event handler
    void process_events();
private:
    void handleEvent(ConnectedEvent event);
    void handleEvent(DisconnectedEvent event);
    void handleEvent(ChannelJoinedEvent event);
    void handleEvent(ChannelLeftEvent event);
    void handleEvent(MessageReceivedEvent event);
private:
    Logger*         m_logger = &Logger::get();
    IrcViewModel&   m_view_model;
    ThreadSafeQueue<NetworkEventVariant>& m_network_event_queue;
};
