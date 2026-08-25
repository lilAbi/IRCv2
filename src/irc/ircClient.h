#pragma once

#include "ircNetworkService.h"
#include "ircMetadata.h"
#include "sessionManager.h"

/*
 *  Interface for the irc client
 */

class IrcClient {
public:
    IrcClient(IrcNetworkService& service, SessionManager& session_manager);
    void connect(ServerConfig config);
    void disconnect(int server_id);
    void joinChannel(int server_id, std::string channel);
    void leaveChannel(int server_id, std::string channel);
    void sendMessage(int server_id, std::string target, std::string message);
private:
    Logger*             m_logger = &Logger::get();
    IrcNetworkService&  m_irc_network_service;
    SessionManager&     m_session_manager;
};
