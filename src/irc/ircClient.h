#pragma once

#include "networkService.h"
#include "ircMetadata.h"
#include "sessionManager.h"

/*
 *  Interface for the irc client
 */

class IrcClient {
public:
    IrcClient(NetworkService& service, SessionManager& session_manager);
    void connect(ServerConfig config);
    void disconnect(int server_id);
    void joinChannel(int server_id, std::string channel);
    void leaveChannel(int server_id, std::string channel);
    void sendMessage(int server_id, std::string target, std::string message);
private:
    Logger*         m_logger = &Logger::get();
    NetworkService& m_network_service;
    SessionManager& m_session_manager;
    int             m_server_id = 0;
};
