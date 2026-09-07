#include "sessionManager.h"

SessionManager::SessionManager(boost::asio::any_io_executor io_executor, ThreadSafeQueue<NetworkEventVariant>& network_event_queue)
    : m_io_executor(io_executor), m_network_event_queue(network_event_queue) {
}

void SessionManager::connect(ServerConfig config) {
    if (m_sessions.contains(config.m_server_id)) {
        m_logger->warn("Session already created for host: {}", config.m_host);
        return;
    }
    m_logger->info("Creating a new IRCSession for host: {}", config.m_host);
    //create a new "IrcSession" object
    const auto new_session = std::make_shared<IrcSession>(m_io_executor, m_network_event_queue);
    //add new session to container
    m_sessions.emplace(config.m_server_id, new_session);
    //start a new connection
    new_session->connect(std::move(config));
}

void SessionManager::disconnect(int server_id) {

}

void SessionManager::join(int server_id, std::string channel) {

}

void SessionManager::leave(int server_id, std::string channel) {

}

void SessionManager::sendMessage(int server_id, std::string target, std::string message) {

}
