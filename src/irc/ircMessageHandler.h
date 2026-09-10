#pragma once

#include "ircMetadata.h"
#include "container/threadSafeQueue.h"

class IrcMessageHandler {
public:
    explicit IrcMessageHandler(ThreadSafeQueue<NetworkEventVariant>& network_event_queue);

    static IrcMessage parseIRCMessage(std::string message);

private:
    ThreadSafeQueue<NetworkEventVariant>& m_network_event_queue;
};