#include "ircClient.h"

IrcClient::IrcClient(NetworkService& service, SessionManager& session_manager)
    : m_network_service(service),   m_session_manager(session_manager) {
}

void IrcClient::connect(ServerConfig config) {
    config.m_server_id = ++m_server_id;

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
    m_network_service.post(
        [this, config = config, logger = Logger::get().get_network_logger()]() mutable {
            logger->info("Submitting registration to Irc Server");
            //need to find a way to get if the
            //grab session object from server id
            const auto& current_session = m_session_manager.m_sessions[config.m_server_id];
            while (true) {
                if (current_session->m_state == SessionState::Disconnected) {
                    logger->warn("Session Disconnected, should not submit registration to IRC Server");
                    break;
                }
                if (current_session->m_state == SessionState::Connected) {
                    logger->info("Session Connection, attempting to register to irc server");
                    break;
                }
            }
        }
   );
}

void IrcClient::disconnect(int server_id) {
}

void IrcClient::joinChannel(int server_id, std::string channel) {
}

void IrcClient::leaveChannel(int server_id, std::string channel) {
}

void IrcClient::sendMessage(int server_id, std::string target, std::string message) {
}
