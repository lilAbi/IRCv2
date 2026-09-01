#include "sessionManager.h"

SessionManager::SessionManager(boost::asio::any_io_executor io_executor, ThreadSafeQueue<NetworkEventVariant>& network_event_queue)
    : m_io_executor(io_executor), m_network_event_queue(network_event_queue) {
}

void SessionManager::connect(ServerConfig config) {
    m_logger->trace("");
    if (m_sessions.contains(config.m_server_id)) {
        m_logger->critical("Session already created for server_id: {}", config.m_server_id);
    } else {
        //make a new session
        m_logger->info("Creating a new IRCSession for host: {}", config.m_host);
        const auto new_sesion = std::make_shared<IrcSession>(m_io_executor, m_network_event_queue);
        m_sessions[config.m_server_id] = new_sesion;
        new_sesion->connect(std::move(config));
    }
}

void SessionManager::disconnect(int server_id) {

}

void SessionManager::join(int server_id, std::string channel) {

}

void SessionManager::leave(int server_id, std::string channel) {

}

void SessionManager::sendMessage(int server_id, std::string target, std::string message) {

}
