#pragma once

#include "ircMetadata.h"
#include "container/threadSafeQueue.h"

class IrcMessageHandler {
public:
    explicit IrcMessageHandler(ThreadSafeQueue<NetworkEventVariant>& network_event_queue);
    //processes an incoming irc command from the server
    void processIRCMessage(std::string message);
private:
    //parse incoming string into irc message abstraction
    IrcMessage parseIRCMessage(std::string& message);
    //route messages to correct handlers
    void dispatchMessage(const IrcMessage& message);
    //handle commands
    void handlePing(const IrcMessage& message);
    void handlePrivateMessage(const IrcMessage& message);
    void handleJoin(const IrcMessage& message);
    void handlePart(const IrcMessage& message);
    void handleNick(const IrcMessage& message);
    void handleWelcome(const IrcMessage& message);
    void handleUnknown(const IrcMessage& message);

private:
    ThreadSafeQueue<NetworkEventVariant>& m_network_event_queue;
};