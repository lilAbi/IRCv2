#include "ircMessageHandler.h"

IrcMessageHandler::IrcMessageHandler(ThreadSafeQueue<NetworkEventVariant> &network_event_queue) : m_network_event_queue(network_event_queue) {
}

IrcMessage IrcMessageHandler::parseIRCMessage(std::string message) {
    IrcMessage irc_message;
    //if message return empty
    if (message.empty()) return irc_message;

    if (message.front() == '@') {
        
    }

    return irc_message;
}
