#include "ircClient.h"

IrcClient::IrcClient(IrcNetworkService& service, SessionManager& session_manager)
    : m_irc_network_service(service),   m_session_manager(session_manager) {
}

void IrcClient::connect(ServerConfig config) {
    m_session_manager.connect({});
}

void IrcClient::disconnect(int server_id) {
}

void IrcClient::joinChannel(int server_id, std::string channel) {
}

void IrcClient::leaveChannel(int server_id, std::string channel) {
}

void IrcClient::sendMessage(int server_id, std::string target, std::string message) {
}
