#include "ircMessageHandler.h"

#include "util/parseCursor.h"

IrcMessageHandler::IrcMessageHandler(ThreadSafeQueue<NetworkEventVariant> &network_event_queue) : m_network_event_queue(network_event_queue) {
}

void IrcMessageHandler::processIRCMessage(std::string message) {
    const auto irc_message = parseIRCMessage(message);
    dispatchMessage(irc_message);
}

IrcMessage IrcMessageHandler::parseIRCMessage(std::string& message) {
    IrcMessage irc_message;
    ParseCursor cursor{message};
    //todo: finish
    return irc_message;
}

void IrcMessageHandler::dispatchMessage(const IrcMessage& message) {
    if (message.m_command == "PING") {
        this->handlePing(message);
    }
    else if (message.m_command == "PRIVMSG") {
        this->handlePrivateMessage(message);
    }
    else if (message.m_command == "JOIN") {
        this->handleJoin(message);
    }
    else if (message.m_command == "PART") {
        this->handlePart(message);
    }
    else if (message.m_command == "NICK") {
        this->handleNick(message);
    }
    else if (message.m_command == "001") {
        this->handleWelcome(message);
    }
    else {
        this->handleUnknown(message);
    }
}
