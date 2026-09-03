#include "ircClient.h"

IrcClient::IrcClient(NetworkService& service, SessionManager& session_manager)
    : m_network_service(service),   m_session_manager(session_manager) {
}

void IrcClient::connect(ServerConfig config) {
    config.m_server_id = ++m_server_id;
    m_logger->info("Submitting a new Connection request");
    m_logger->debug("Host: {}", config.m_host);
    m_logger->debug("Port: {}", config.m_port);
    m_logger->debug("Nick: {}", config.m_nick);
    m_logger->debug("Username: {}", config.m_username);
    m_logger->debug("Server ID: {}", config.m_server_id);
    //attempt to connect to the server
    m_network_service.post(
        [this, config = config]() mutable {
            m_session_manager.connect(std::move(config));
        }
    );
    //submit irc registration protocol
    /*
    m_network_service.post(
       [this, config = std::move(config)]() mutable {
           m_session_manager.connect(std::move(config));
       }
   );
   */
}

void IrcClient::disconnect(int server_id) {
}

void IrcClient::joinChannel(int server_id, std::string channel) {
}

void IrcClient::leaveChannel(int server_id, std::string channel) {
}

void IrcClient::sendMessage(int server_id, std::string target, std::string message) {
}
